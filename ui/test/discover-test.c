
#include <stdio.h>

#include "ui/common/discover-client.h"

static const char *device_type_string(enum device_type type)
{
	switch (type) {
	case DEVICE_TYPE_DISK:
		return "disk";
	case DEVICE_TYPE_USB:
		return "usb";
	case DEVICE_TYPE_NETWORK:
		return "network";
	case DEVICE_TYPE_OPTICAL:
		return "optical";
	case DEVICE_TYPE_ANY:
		return "any";
	case DEVICE_TYPE_LUKS:
		return "encrypted";
	case DEVICE_TYPE_UNKNOWN:
		return "unknown";
	}
	return "invalid";
}

static int print_device_add(struct device *device,
	void __attribute__((unused)) *arg)
{
	struct boot_option *opt;

	printf("new device:\n");
	printf("\tid:   %s\n", device->id);
	printf("\ttype: %s\n", device_type_string(device->type));
	printf("\tname: %s\n", device->name);
	printf("\tdesc: %s\n", device->description);
	printf("\ticon: %s\n", device->icon_file);

	printf("\tboot options:\n");
	list_for_each_entry(&device->boot_options, opt, list) {
		printf("\t\tid:   %s\n", opt->id);
		printf("\t\tname: %s\n", opt->name);
		printf("\t\tdesc: %s\n", opt->description);
		printf("\t\ticon: %s\n", opt->icon_file);
		printf("\t\tboot: %s\n", opt->boot_image_file);
		printf("\t\tinit: %s\n", opt->initrd_file);
		printf("\t\tdtb:  %s\n", opt->dtb_file);
		printf("\t\targs: %s\n", opt->boot_args);
	}

	return 0;
}

static int print_boot_option_add(struct device *dev,
		struct boot_option *opt,
		void __attribute__((unused)) *arg)
{
	printf("new boot option (dev: %s):\n", dev->id);
	printf("\tdev id: %s\n", opt->device_id);
	printf("\tid:     %s\n", opt->id);
	printf("\tname:   %s\n", opt->name);
	printf("\tdesc:   %s\n", opt->description);
	printf("\ticon:   %s\n", opt->icon_file);
	printf("\tboot:   %s\n", opt->boot_image_file);
	printf("\tinit:   %s\n", opt->initrd_file);
	printf("\targs:   %s\n", opt->boot_args);
	printf("\tdefault:%d\n", opt->is_default);

	return 0;
}

static void print_device_remove(struct device *device,
	void __attribute__((unused)) *arg)
{
	printf("removed device:\n");
	printf("\tid:   %s\n", device->id);
	printf("\tname: %s\n", device->name);
}

static void print_status(struct status *status,
	void __attribute__((unused)) *arg)
{
	printf("status:\n");
	printf("\ttype:     %d\n", status->type);
	printf("\tmessage:  %s\n", status->message);

}

static void print_sysinfo(struct system_info *sysinfo,
	void __attribute__((unused)) *arg)
{
	unsigned int i;

	printf("sysinfo:\n");
	printf("\ttype:     %s\n", sysinfo->type);
	printf("\tid:       %s\n", sysinfo->identifier);

	if (sysinfo->n_interfaces == 0)
		printf("\tno interfaces.\n");
	else
		printf("\tinterfaces:\n");

	for (i = 0; i < sysinfo->n_interfaces; i++) {
		struct interface_info *if_info = sysinfo->interfaces[i];
		uint8_t *m = if_info->hwaddr;

		printf("\t\tname:   %s\n", if_info->name);

		if (if_info->hwaddr_size == 6)
			printf("\t\tmac:    %02x:%02x:%02x:%02x:%02x:%02x\n",
					m[0], m[1], m[2], m[3], m[4], m[5]);
		else
			printf("\t\tmac:    unknown hwaddr size %d\n",
					if_info->hwaddr_size);
	}
}

static struct discover_client_ops client_ops = {
	.device_add = print_device_add,
	.boot_option_add = print_boot_option_add,
	.device_remove = print_device_remove,
	.update_status = print_status,
	.update_sysinfo = print_sysinfo,
};

int main(void)
{
	struct discover_client *client;
	struct waitset *waitset;

	waitset = waitset_create(NULL);

	client = discover_client_init(waitset, &client_ops, NULL);
	if (!client)
		return -1;

	for (;;) {
		int rc;

		rc = waiter_poll(waitset);
		if (rc)
			break;
	}

	return 0;
}
