# uboot

## uboot编译介绍

### U-Boot 是一种开源的引导程序，用于引导 Linux 内核和其他操作系统。要操作 U-Boot 源码，首先需要下载 U-Boot 源码，然后进行编译和调试。以下是一些基本的步骤：

### 下载 U-Boot 源码：首先，访问 U-Boot 的官方网站（https://www.denx.de/wiki/U-Boot），然后从下载页面下载最新的源代码。

### 解压源码：将下载的源码解压到一个方便的目录中，例如  ~/uboot 。

### 配置 U-Boot：进入 U-Boot 源码目录，然后运行  make menuconfig  命令以配置 U-Boot。根据您的硬件平台和需求进行相应的设置。

### 编译 U-Boot：在 U-Boot 源码目录中运行  make  命令以编译 U-Boot。这将生成一个名为  u-boot.bin  的二进制文件。

### 烧写 U-Boot 到目标板：将  u-boot.bin  文件烧写到目标板（例如开发板）的特定存储设备上，如 NAND Flash 或 eMMC。具体方法取决于您的硬件平台和烧写工具。

### 测试 U-Boot：烧写完毕后，启动目标板，确保 U-Boot 能够正常工作，例如检查串口输出或通过网络接口登录目标板。

## 如何在 U-Boot 中添加一个选择菜单。已熟悉 U-Boot 的基本结构和编译过程。

### 编辑菜单配置文件：

- 打开 U-Boot 源码目录中的  configs/<your_board>.h  文件（例如  configs/x210_sd.h ），然后在文件中找到以下宏定义：

	- #define CONFIG_BOOTDELAY        3

	- #define CONFIG_BOOTMENU         "Boot Menu"

- 将  CONFIG_BOOTMENU  的值更改为您想要的菜单名称，例如：

	- #define CONFIG_BOOTMENU         "My Custom Boot Menu"

### 添加菜单项：

- 在  configs/<your_board>.h  文件中，找到以下宏定义：

	- #define CONFIG_BOOTCOMMAND       "bootcmd"

	- 将  CONFIG_BOOTCOMMAND  的值更改为您想要的菜单项命令，例如：

	- #define CONFIG_BOOTCOMMAND       "bootcmd_menu"

- 然后，在 U-Boot 源码目录的  common/bootcmd.c  文件中，找到  do_bootcmd  函数。在这个函数中，添加一个新的 case 语句来处理您的菜单项命令：

	- static int do_bootcmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])

	- {

	-     ...

	-     switch (cmd) {

	-         ...

	-         case BOOTCMD_MENU: // 添加新的 case 语句来处理您的菜单项命令

	-             menu_main(); // 调用 menu_main 函数以显示菜单界面

	-             break;  // 添加 break 语句以防止 fall-through 到其他 case 语句中。   }   return 1; }

### 实现菜单函数：

- 在 U-Boot 源码目录的  common/menu.c  文件中，实现一个名为  menu_main  的函数，以显示您的自定义菜单。这个函数可以如下所示：

	- void menu_main(void)

	- {

	-     printf("My Custom Boot Menu\n");

	-     printf("1. Boot Linux\n");

	-     printf("2. Boot Android\n");

	-     printf("3. Exit\n");

	-     printf("Enter your choice: ");

	-     int choice = get_menu_choice(); // 获取用户输入的菜单选项，您可以根据需要实现此函数。

	-     switch (choice) {

	-         case 1: // 根据用户选择的菜单选项执行相应的操作。例如，在这里我们可以启动 Linux。

	-             run_command("bootcmd_linux", 0); // 假设您已经定义了一个名为 "bootcmd_linux" 的命令来启动 Linux。

	-             break;

	-         case 2: // 在这里我们可以启动 Android。您可以根据需要定义其他命令。

	-             run_command("bootcmd_android", 0); // 假设您已经定义了一个名为 "bootcmd_android" 的命令来启动 Android。

	-             break;

	-         case 3: // 用户选择退出菜单。在这种情况下，我们可以简单地返回到 U-Boot 命令行。

	-             return; // 返回到 U-Boot 命令行。不需要调用 run_command 函数，因为它可能会导致无限循环。 default: // 用户输入无效的选项时执行默认操作。在这里，我们可以简单地显示一个错误消息并返回到菜单。 printf("Invalid choice.\n"); menu_main(); // 显示菜单并等待用户输入新的选项。 } }

### 更新 U-Boot 配置文件：

- 在 U-Boot 源码目录的  include/configs/<您的板子>.h  文件中，您需要更新  CONFIG_EXTRA_ENV_SETTINGS  变量以包含您的自定义菜单环境变量。例如：

	- #define CONFIG_EXTRA_ENV_SETTINGS \

	-     "my_custom_menu=My Custom Boot Menu\0" \

	-     "my_custom_menu_timeout=10\0" \

	-     "my_custom_menu_default=1\0" \

### 编译并烧写 U-Boot：

- 在 U-Boot 源码目录中，使用以下命令编译并烧写 U-Boot：

	- make <您的板子>_config

	- make -j4 all && make <您的板子>_clean && make <您的板子>_flashall

### 测试自定义菜单：

- 将开发板连接到 PC，然后启动开发板。您应该看到您的自定义菜单。输入数字并按 Enter 键以选择相应的选项。如果一切正常，您应该能够启动 Linux 或 Android，或者退出到 U-Boot 命令行。

