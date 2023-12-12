/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Enable crypt/password support */
/* #undef CRYPT_SUPPORT */

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#define ENABLE_NLS 1

/* Enable hard lockdown */
/* #undef HARD_LOCKDOWN */

/* Define to 1 if you have the MacOS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the MacOS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Define to 1 if a SysV or X/Open compatible Curses library is present */
/* #undef HAVE_CURSES */

/* Define to 1 if library supports color (enhanced functions) */
/* #undef HAVE_CURSES_COLOR */

/* Define to 1 if library supports X/Open Enhanced functions */
/* #undef HAVE_CURSES_ENHANCED */

/* Define to 1 if <curses.h> is present */
/* #undef HAVE_CURSES_H */

/* Define to 1 if library supports certain obsolete features */
/* #undef HAVE_CURSES_OBSOLETE */

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <elfutils/libdw.h> header file. */
#define HAVE_ELFUTILS_LIBDW_H 1

/* Define to 1 if the Curses Form library is present */
/* #undef HAVE_FORM */

/* Define to 1 if <form.h> is present */
/* #undef HAVE_FORM_H */

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define if you have the iconv() function and it works. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <libflash/libflash.h> header file. */
/* #undef HAVE_LIBFLASH_LIBFLASH_H */

/* Define to 1 if you have the `twin' library (-ltwin). */
/* #undef HAVE_LIBTWIN */

/* Define to 1 if you have the <libtwin/twin_fbdev.h> header file. */
/* #undef HAVE_LIBTWIN_TWIN_FBDEV_H */

/* Define to 1 if you have the <libtwin/twin_x11.h> header file. */
/* #undef HAVE_LIBTWIN_TWIN_X11_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if the Curses Menu library is present */
/* #undef HAVE_MENU */

/* Define to 1 if <menu.h> is present */
/* #undef HAVE_MENU_H */

/* Define to 1 if the Ncurses library is present */
/* #undef HAVE_NCURSES */

/* Define to 1 if the NcursesW library is present */
/* #undef HAVE_NCURSESW */

/* Define to 1 if <ncursesw/curses.h> is present */
/* #undef HAVE_NCURSESW_CURSES_H */

/* Define to 1 if <ncursesw/form.h> is present */
/* #undef HAVE_NCURSESW_FORM_H */

/* Define to 1 if <ncursesw.h> is present */
/* #undef HAVE_NCURSESW_H */

/* Define to 1 if <ncursesw/menu.h> is present */
/* #undef HAVE_NCURSESW_MENU_H */

/* Define to 1 if <ncurses/curses.h> is present */
/* #undef HAVE_NCURSES_CURSES_H */

/* Define to 1 if <ncurses/form.h> is present */
/* #undef HAVE_NCURSES_FORM_H */

/* Define to 1 if <ncurses.h> is present */
/* #undef HAVE_NCURSES_H */

/* Define to 1 if <ncurses/menu.h> is present */
/* #undef HAVE_NCURSES_MENU_H */

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <varargs.h> header file. */
/* #undef HAVE_VARARGS_H */

/* Whether va_copy() is available */
#define HAVE_VA_COPY 1

/* Whether __va_copy() is available */
/* #undef HAVE___VA_COPY */

/* Path to "cp" on the host */
#define HOST_PROG_CP "/bin/cp"

/* Path to "CRYPTSETUP" on the host */
#define HOST_PROG_CRYPTSETUP "/usr/sbin/cryptsetup"

/* Path to "dmidecode" on the host */
#define HOST_PROG_DMIDECODE "/sbin/dmidecode"

/* Path to "ip" on the host */
#define HOST_PROG_IP "/sbin/ip"

/* Path to "kexec" on the host */
#define HOST_PROG_KEXEC "/sbin/kexec"

/* Path to "mount" on the host */
#define HOST_PROG_MOUNT "/bin/mount"

/* Path to "pb-exec" on the host */
#define HOST_PROG_PB_EXEC "/usr/sbin/pb-exec"

/* Path to "pb-plugin" on the host */
#define HOST_PROG_PB_PLUGIN "/usr/sbin/pb-plugin"

/* Path to "scsi-rescan" on the host */
#define HOST_PROG_SCSI_RESCAN "/usr/sbin/scsi-rescan"

/* Path to "sftp" on the host */
#define HOST_PROG_SFTP "/usr/bin/sftp"

/* Path to "sh" on the host */
#define HOST_PROG_SH "/bin/sh"

/* Path to "shutdown" on the host */
#define HOST_PROG_SHUTDOWN "/sbin/shutdown"

/* Path to "tftp" on the host */
#define HOST_PROG_TFTP "/usr/bin/tftp"

/* Path to "udhcpc" on the host */
#define HOST_PROG_UDHCPC "/sbin/udhcpc"

/* Path to "udhcpc6" on the host */
#define HOST_PROG_UDHCPC6 "/usr/bin/udhcpc6"

/* Path to "umount" on the host */
#define HOST_PROG_UMOUNT "/bin/umount"

/* Path to "vgchange" on the host */
#define HOST_PROG_VGCHANGE "/usr/sbin/vgchange"

/* Path to "vgscan" on the host */
#define HOST_PROG_VGSCAN "/usr/sbin/vgscan"

/* Path to "wget" on the host */
#define HOST_PROG_WGET "/usr/bin/wget"

/* gpgme home dir */
#define KEYRING_PATH "/etc/gpg"

/* Lockdown file location */
#define LOCKDOWN_FILE "/etc/pb-lockdown"

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Enable MTD support */
/* #undef MTD_SUPPORT */

/* Name of package */
#define PACKAGE "petitboot"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "https://lists.ozlabs.org/listinfo/petitboot"

/* Define to the full name of this package. */
#define PACKAGE_NAME "petitboot"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "petitboot v1.14"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "petitboot"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "v1.14"

/* Define if you have signed boot enabled */
/* #undef SIGNED_BOOT */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* tftp client type */
#define TFTP_TYPE TFTP_TYPE_UNKNOWN

/* Support old udev logging interface */
/* #undef UDEV_LOGGING */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* openssl verify dgst */
#define VERIFY_DIGEST "sha256"

/* Version number of package */
#define VERSION "v1.14"

/* Busybox environment enabled */
/* #undef WITH_BUSYBOX */

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */
