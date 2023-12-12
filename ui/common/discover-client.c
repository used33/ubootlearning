
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <asm/byteorder.h>

#include <talloc/talloc.h>
#include <log/log.h>

#include "discover-client.h"
#include "pb-protocol/pb-protocol.h"

struct discover_client {
	int fd;
	struct discover_client_ops ops;
	int n_devices;
	struct device **devices;
	bool authenticated;
};

static int discover_client_destructor(void *arg)
{
	struct discover_client *client = arg;

	if (client->fd >= 0)
		close(client->fd);

	return 0;
}

void discover_client_destroy(struct discover_client *client)
{
	talloc_free(client);
}

static struct device *find_device(struct discover_client *client,
		const char *id)
{
	int i;

	for (i = 0; i < client->n_devices; i++) {
		struct device *dev = client->devices[i];
		if (!strcmp(dev->id, id))
			return dev;
	}

	return NULL;
}

static void device_add(struct discover_client *client, struct device *device)
{
	client->n_devices++;
	client->devices = talloc_realloc(client, client->devices,
			struct device *, client->n_devices);

	client->devices[client->n_devices - 1] = device;
	talloc_steal(client, device);
	list_init(&device->boot_options);

	if (client->ops.device_add)
		client->ops.device_add(device, client->ops.cb_arg);
}

static void boot_option_add(struct discover_client *client,
		struct boot_option *opt)
{
	struct device *dev;

	dev = find_device(client, opt->device_id);

	/* we require that devices are already present before any boot options
	 * are added */
	assert(dev);

	talloc_steal(dev, opt);
	list_add(&dev->boot_options, &opt->list);

	if (client->ops.boot_option_add)
		client->ops.boot_option_add(dev, opt, client->ops.cb_arg);
}

static void device_remove(struct discover_client *client, const char *id)
{
	struct device *device = NULL;
	int i;

	for (i = 0; i < client->n_devices; i++) {
		if (!strcmp(client->devices[i]->id, id)) {
			device = client->devices[i];
			break;
		}
	}

	if (!device)
		return;

	/* remove the device from the client's device array */
	client->n_devices--;
	memmove(&client->devices[i], &client->devices[i+1],
			(client->n_devices - i) * sizeof(client->devices[0]));
	client->devices = talloc_realloc(client, client->devices,
			struct device *, client->n_devices);

	/* notify the UI */
	client->ops.device_remove(device, client->ops.cb_arg);

	talloc_free(device);
}

static void plugin_option_add(struct discover_client *client,
		struct plugin_option *opt)
{
	talloc_steal(client, opt);

	if (client->ops.plugin_option_add)
		client->ops.plugin_option_add(opt, client->ops.cb_arg);
}

static void plugins_remove(struct discover_client *client)
{
	if (client->ops.plugins_remove)
		client->ops.plugins_remove(client->ops.cb_arg);
}

void discover_client_enumerate(struct discover_client *client)
{
	struct boot_option *opt;
	struct device *device;
	int i;

	for (i = 0; i < client->n_devices; i++) {
		device = client->devices[i];
		if (client->ops.device_add)
			client->ops.device_add(device, client->ops.cb_arg);

		list_for_each_entry(&device->boot_options, opt, list)
			if (client->ops.boot_option_add)
				client->ops.boot_option_add(device, opt,
						client->ops.cb_arg);
	}
}

static void update_status(struct discover_client *client,
		struct status *status)
{
	if (client->ops.update_status)
		client->ops.update_status(status, client->ops.cb_arg);
}

static void update_sysinfo(struct discover_client *client,
		struct system_info *sysinfo)
{
	if (client->ops.update_sysinfo)
		client->ops.update_sysinfo(sysinfo, client->ops.cb_arg);
}

static void update_config(struct discover_client *client,
		struct config *config)
{
	if (client->ops.update_config)
		client->ops.update_config(config, client->ops.cb_arg);
}

static int discover_client_process(void *arg)
{
	struct discover_client *client = arg;
	struct pb_protocol_message *message;
	struct auth_message *auth_msg;
	struct plugin_option *p_opt;
	struct system_info *sysinfo;
	struct boot_option *opt;
	struct status *status;
	struct config *config;
	struct device *dev;
	char *dev_id;
	void *ctx;
	int rc;

	/* We use a temporary context for processing one message; persistent
	 * data is re-parented to the client in the callbacks. */
	ctx = talloc_new(client);

	message = pb_protocol_read_message(ctx, client->fd);

	if (!message)
		return -1;

	switch (message->action) {
	case PB_PROTOCOL_ACTION_DEVICE_ADD:
		dev = talloc_zero(ctx, struct device);
		list_init(&dev->boot_options);

		rc = pb_protocol_deserialise_device(dev, message);
		if (rc) {
			pb_log_fn("no device?\n");
			goto out;
		}

		device_add(client, dev);
		break;
	case PB_PROTOCOL_ACTION_BOOT_OPTION_ADD:
		opt = talloc_zero(ctx, struct boot_option);

		rc = pb_protocol_deserialise_boot_option(opt, message);
		if (rc) {
			pb_log_fn("no boot_option?\n");
			goto out;
		}

		boot_option_add(client, opt);
		break;
	case PB_PROTOCOL_ACTION_DEVICE_REMOVE:
		dev_id = pb_protocol_deserialise_string(ctx, message);
		if (!dev_id) {
			pb_log_fn("no device id?\n");
			goto out;
		}
		device_remove(client, dev_id);
		break;
	case PB_PROTOCOL_ACTION_STATUS:
		status = talloc_zero(ctx, struct status);

		rc = pb_protocol_deserialise_boot_status(status, message);
		if (rc) {
			pb_log_fn("invalid status message?\n");
			goto out;
		}
		update_status(client, status);
		break;
	case PB_PROTOCOL_ACTION_SYSTEM_INFO:
		sysinfo = talloc_zero(ctx, struct system_info);

		rc = pb_protocol_deserialise_system_info(sysinfo, message);
		if (rc) {
			pb_log_fn("invalid sysinfo message?\n");
			goto out;
		}
		update_sysinfo(client, sysinfo);
		break;
	case PB_PROTOCOL_ACTION_CONFIG:
		config = talloc_zero(ctx, struct config);

		rc = pb_protocol_deserialise_config(config, message);
		if (rc) {
			pb_log_fn("invalid config message?\n");
			goto out;
		}
		update_config(client, config);
		break;
	case PB_PROTOCOL_ACTION_PLUGIN_OPTION_ADD:
		p_opt = talloc_zero(ctx, struct plugin_option);

		rc = pb_protocol_deserialise_plugin_option(p_opt, message);
		if (rc) {
			pb_log_fn("no plugin_option?\n");
			goto out;
		}

		plugin_option_add(client, p_opt);
		break;
	case PB_PROTOCOL_ACTION_PLUGINS_REMOVE:
		plugins_remove(client);
		break;
	case PB_PROTOCOL_ACTION_AUTHENTICATE:
		auth_msg = talloc_zero(ctx, struct auth_message);

		rc = pb_protocol_deserialise_authenticate(auth_msg, message);
		if (rc || auth_msg->op != AUTH_MSG_RESPONSE) {
			pb_log("%s: invalid auth message? (%d)\n",
					__func__, rc);
			goto out;
		}

		pb_log("Client %sauthenticated by server\n",
				client->authenticated ? "" : "un");
		client->authenticated = auth_msg->authenticated;
		break;
	default:
		pb_log_fn("unknown action %d\n", message->action);
	}

out:
	talloc_free(ctx);

	return 0;
}

struct discover_client* discover_client_init(struct waitset *waitset,
	const struct discover_client_ops *ops, void *cb_arg)
{
	struct discover_client *client;
	struct sockaddr_un addr;

	client = talloc(NULL, struct discover_client);
	if (!client)
		return NULL;

	memcpy(&client->ops, ops, sizeof(client->ops));
	client->ops.cb_arg = cb_arg;

	client->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client->fd < 0) {
		pb_log_fn("socket: %s\n", strerror(errno));
		goto out_err;
	}

	talloc_set_destructor(client, discover_client_destructor);

	client->n_devices = 0;
	client->devices = NULL;

	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, PB_SOCKET_PATH);

	if (connect(client->fd, (struct sockaddr *)&addr, sizeof(addr))) {
		pb_log_fn("connect: %s\n", strerror(errno));
		goto out_err;
	}

	waiter_register_io(waitset, client->fd, WAIT_IN,
			discover_client_process, client);

	/* Assume this client can't make changes if crypt support is enabled */
#ifdef CRYPT_SUPPORT
	client->authenticated = false;
#else
	client->authenticated = true;
#endif

	return client;

out_err:
	talloc_free(client);
	return NULL;
}

/* accessors for discovered devices */
int discover_client_device_count(struct discover_client *client)
{
	return client->n_devices;
}

struct device *discover_client_get_device(struct discover_client *client,
		int index)
{
	if (index < 0 || index >= client->n_devices)
		return NULL;

	return client->devices[index];
}

bool discover_client_authenticated(struct discover_client *client)
{
	return client->authenticated;
}

static void create_boot_command(struct boot_command *command,
		const struct device *device __attribute__((unused)),
		const struct boot_option *boot_option,
		const struct pb_boot_data *data)
{
	command->option_id = boot_option ? boot_option->id : NULL;
	command->boot_image_file = data->image;
	command->initrd_file = data->initrd;
	command->dtb_file = data->dtb;
	command->boot_args = data->args;
	command->args_sig_file = data->args_sig_file;
	command->console = ttyname(STDIN_FILENO);
}

int discover_client_boot(struct discover_client *client,
		const struct device *device,
		const struct boot_option *boot_option,
		const struct pb_boot_data *data)
{
	struct pb_protocol_message *message;
	struct boot_command boot_command;
	int len, rc;

	create_boot_command(&boot_command, device, boot_option, data);

	len = pb_protocol_boot_len(&boot_command);

	message = pb_protocol_create_message(client,
			PB_PROTOCOL_ACTION_BOOT, len);

	if (!message)
		return -1;

	pb_protocol_serialise_boot_command(&boot_command,
			message->payload, len);

	rc = pb_protocol_write_message(client->fd, message);

	return rc;
}

int discover_client_cancel_default(struct discover_client *client)
{
	struct pb_protocol_message *message;

	message = pb_protocol_create_message(client,
			PB_PROTOCOL_ACTION_CANCEL_DEFAULT, 0);

	if (!message)
		return -1;

	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_reinit(struct discover_client *client)
{
	struct pb_protocol_message *message;

	message = pb_protocol_create_message(client,
			PB_PROTOCOL_ACTION_REINIT, 0);

	if (!message)
		return -1;

	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_config(struct discover_client *client,
		struct config *config)
{
	struct pb_protocol_message *message;
	int len;

	len = pb_protocol_config_len(config);

	message = pb_protocol_create_message(client,
				PB_PROTOCOL_ACTION_CONFIG, len);
	if (!message)
		return -1;

	pb_protocol_serialise_config(config, message->payload, len);

	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_url(struct discover_client *client,
		char *url)
{
	struct pb_protocol_message *message;
	int len;

	len = pb_protocol_url_len(url);

	message = pb_protocol_create_message(client,
				PB_PROTOCOL_ACTION_ADD_URL, len);
	if (!message)
		return -1;

	pb_protocol_serialise_url(url, message->payload, len);

	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_plugin_install(struct discover_client *client,
		char *file)
{
	struct pb_protocol_message *message;
	int len;

	len = pb_protocol_url_len(file);

	message = pb_protocol_create_message(client,
				PB_PROTOCOL_ACTION_PLUGIN_INSTALL, len);
	if (!message)
		return -1;

	pb_protocol_serialise_url(file, message->payload, len);

	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_temp_autoboot(struct discover_client *client,
		const struct autoboot_option *opt)
{
	struct pb_protocol_message *message;
	int len;

	len = pb_protocol_temp_autoboot_len(opt);

	message = pb_protocol_create_message(client,
			PB_PROTOCOL_ACTION_TEMP_AUTOBOOT, len);
	if (!message)
		return -1;

	pb_protocol_serialise_temp_autoboot(opt, message->payload, len);

	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_authenticate(struct discover_client *client,
		char *password)
{
	struct pb_protocol_message *message;
	struct auth_message auth_msg;
	int len;

	auth_msg.op = AUTH_MSG_REQUEST;
	auth_msg.password = password;

	len = pb_protocol_authenticate_len(&auth_msg);

	message = pb_protocol_create_message(client,
				PB_PROTOCOL_ACTION_AUTHENTICATE, len);
	if (!message)
		return -1;

	pb_log("serialising auth message..\n");
	pb_protocol_serialise_authenticate(&auth_msg, message->payload, len);

	pb_log("sending auth message..\n");
	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_set_password(struct discover_client *client,
		char *password, char *new_password)
{
	struct pb_protocol_message *message;
	struct auth_message auth_msg;
	int len;

	auth_msg.op = AUTH_MSG_SET;
	auth_msg.set_password.password = password;
	auth_msg.set_password.new_password = new_password;

	len = pb_protocol_authenticate_len(&auth_msg);

	message = pb_protocol_create_message(client,
				PB_PROTOCOL_ACTION_AUTHENTICATE, len);
	if (!message)
		return -1;

	pb_log("serialising auth message..\n");
	pb_protocol_serialise_authenticate(&auth_msg, message->payload, len);

	pb_log("sending auth message..\n");
	return pb_protocol_write_message(client->fd, message);
}

int discover_client_send_open_luks_device(struct discover_client *client,
		char *password, char *device_id)
{
	struct pb_protocol_message *message;
	struct auth_message auth_msg;
	int len;

	auth_msg.op = AUTH_MSG_DECRYPT;
	auth_msg.decrypt_dev.password = password;
	auth_msg.decrypt_dev.device_id = device_id;

	len = pb_protocol_authenticate_len(&auth_msg);

	message = pb_protocol_create_message(client,
				PB_PROTOCOL_ACTION_AUTHENTICATE, len);
	if (!message)
		return -1;

	pb_log("serialising auth message..\n");
	pb_protocol_serialise_authenticate(&auth_msg, message->payload, len);

	pb_log("sending auth message..\n");
	return pb_protocol_write_message(client->fd, message);
}
