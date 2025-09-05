// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "pico/time.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/stdio_uart.h"

#include "hardware/pll.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#include <hagl_hal.h>
#include <hagl/bitmap.h>
#include <hagl.h>

#define COLS 80
#define ROWS 40
#define TIME_PREFIX_LEN 17

static int line_num = 0;
static int offset = 0;
const int line_width = sizeof(wchar_t) * COLS;
wchar_t console_buf_font8x8[COLS * ROWS];

static char *logs[] = {
"Starting kernel ...",
" ",
"Booting Linux on physical CPU 0x0",
"Linux version 6.16 (developer@DESKTOP-D4KIBMK) (arm-linux-gnueabihf-gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #1 Mon Feb 17 13:35:52 +08 2025",
"CPU: ARM926EJ-S [41069265] revision 5 (ARMv5TEJ), cr=0005317f",
"CPU: VIVT data cache, VIVT instruction cache",
"OF: fdt: Machine model: Lectech Pi F1C200s",
"Memory policy: Data cache writeback",
"Zone ranges:",
"  Normal   [mem 0x0000000080000000-0x0000000083dfffff]",
"Movable zone start for each node",
"Early memory node ranges",
"  node   0: [mem 0x0000000080000000-0x0000000083dfffff]",
"Initmem setup node 0 [mem 0x0000000080000000-0x0000000083dfffff]",
"OF: reserved mem: Reserved memory: No reserved-memory node in the DT",
"Kernel command line: ",
"printk: log buffer data + meta data: 131072 + 409600 = 540672 bytes",
"Dentry cache hash table entries: 8192 (order: 3, 32768 bytes, linear)",
"Inode-cache hash table entries: 4096 (order: 2, 16384 bytes, linear)",
"Built 1 zonelists, mobility grouping on.  Total pages: 15872",
"mem auto-init: stack:off, heap alloc:off, heap free:off",
"SLUB: HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1",
"NR_IRQS: 16, nr_irqs: 16, preallocated irqs: 16",
"sched_clock: 32 bits at 24MHz, resolution 41ns, wraps every 89478484971ns",
"clocksource: timer: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 79635851949 ns",
"Console: colour dummy device 80x30",
"printk: legacy console [tty0] enabled",
"Calibrating delay loop... 203.16 BogoMIPS (lpj=1015808)",
"CPU: Testing write buffer coherency: ok",
"pid_max: default: 32768 minimum: 301",
"Mount-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)",
"Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)",
"Setting up static identity map for 0x80100000 - 0x8010003c",
"Memory: 50260K/63488K available (7168K kernel code, 654K rwdata, 1992K rodata, 1024K init, 254K bss, 12840K reserved, 0K cma-reserved)",
"devtmpfs: initialized",
"clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns",
"futex hash table entries: 256 (order: -1, 3072 bytes, linear)",
"pinctrl core: initialized pinctrl subsystem",
"NET: Registered PF_NETLINK/PF_ROUTE protocol family",
"DMA: preallocated 256 KiB pool for atomic coherent allocations",
"thermal_sys: Registered thermal governor 'step_wise'",
"thermal_sys: Registered thermal governor 'user_space'",
"cpuidle: using governor menu",
"SCSI subsystem initialized",
"usbcore: registered new interface driver usbfs",
"usbcore: registered new interface driver hub",
"usbcore: registered new device driver usb",
"clocksource: Switched to clocksource timer",
"NET: Registered PF_INET protocol family",
"IP idents hash table entries: 2048 (order: 2, 16384 bytes, linear)",
"tcp_listen_portaddr_hash hash table entries: 1024 (order: 0, 4096 bytes, linear)",
"Table-perturb hash table entries: 65536 (order: 6, 262144 bytes, linear)",
"TCP established hash table entries: 1024 (order: 0, 4096 bytes, linear)",
"TCP bind hash table entries: 1024 (order: 1, 8192 bytes, linear)",
"TCP: Hash tables configured (established 1024 bind 1024)",
"UDP hash table entries: 256 (order: 1, 8192 bytes, linear)",
"UDP-Lite hash table entries: 256 (order: 1, 8192 bytes, linear)",
"NET: Registered PF_UNIX/PF_LOCAL protocol family",
"RPC: Registered named UNIX socket transport module.",
"RPC: Registered udp transport module.",
"RPC: Registered tcp transport module.",
"RPC: Registered tcp-with-tls transport module.",
"RPC: Registered tcp NFSv4.1 backchannel transport module.",
"Initialise system trusted keyrings",
"workingset: timestamp_bits=30 max_order=14 bucket_order=0",
"Key type asymmetric registered",
"Asymmetric key parser 'x509' registered",
"Block layer SCSI generic (bsg) driver version 0.4 loaded (major 252)",
"io scheduler mq-deadline registered",
"io scheduler kyber registered",
"Serial: 8250/16550 driver, 8 ports, IRQ sharing disabled",
"[drm] Initialized simpledrm 1.0.0 for 83e00000.framebuffer on minor 0",
"Console: switching to colour frame buffer device 60x50",
"simple-framebuffer 83e00000.framebuffer: [drm] fb0: simpledrmdrmfb frame buffer device",
"usbcore: registered new interface driver rtl8xxxu",
"usbcore: registered new interface driver usb-storage",
"i2c_dev: i2c /dev entries driver",
"sunxi-wdt 1c20ca0.watchdog: Watchdog enabled (timeout=16 sec, nowayout=0)",
"usbcore: registered new interface driver usbhid",
"usbhid: USB HID core driver",
"NET: Registered PF_INET6 protocol family",
"Segment Routing with IPv6",
"In-situ OAM (IOAM) with IPv6",
"sit: IPv6, IPv4 and MPLS over IPv4 tunneling driver",
"NET: Registered PF_PACKET protocol family",
"Key type dns_resolver registered",
"Loading compiled-in X.509 certificates",
"gpio gpiochip0: Static allocation of GPIO base is deprecated, use dynamic allocation.",
"suniv-f1c100s-pinctrl 1c20800.pinctrl: initialized sunXi PIO driver",
"suniv-f1c100s-pinctrl 1c20800.pinctrl: supply vcc-pa not found, using dummy regulator",
"1c25400.serial: ttyS0 at MMIO 0x1c25400 (irq = 116, base_baud = 6250000) is a 16550A",
"printk: legacy console [ttyS0] enabled",
"suniv-f1c100s-pinctrl 1c20800.pinctrl: supply vcc-pc not found, using dummy regulator",
"sun6i-spi 1c05000.spi: Failed to request TX DMA channel",
"sun6i-spi 1c05000.spi: Failed to request RX DMA channel",
"spi-nand spi0.0: Winbond SPI NAND was found.",
"spi-nand spi0.0: 128 MiB, block size: 128 KiB, page size: 2048, OOB size: 64",
"usb_phy_generic usb_phy_generic.0.auto: dummy supplies not allowed for exclusive requests (id=vbus)",
"suniv-f1c100s-pinctrl 1c20800.pinctrl: supply vcc-pf not found, using dummy regulator",
"cfg80211: Loading compiled-in X.509 certificates for regulatory database",
"Loaded X.509 cert 'sforshee: 00b28ddf47aef9cea7'",
"sunxi-mmc 1c0f000.mmc: initialized, max. request size: 16384 KB",
"Loaded X.509 cert 'wens: 61c038651aabdcf94bd0ac7ff06c7248db18c600'",
"clk: Disabling unused clocks",
"platform regulatory.0: Direct firmware load for regulatory.db failed with error -2",
"cfg80211: failed to load regulatory.db",
"List of all partitions:",
"1f00          131072 mtdblock0 ",
" (driver?)",
"No filesystem could mount root, tried: ",
"",
"Kernel panic - not syncing: VFS: Unable to mount root fs on '' or unknown-block(0,0)",
"CPU: 0 UID: 0 PID: 1 Comm: swapper Not tainted 6.16 #1",
"Hardware name: Allwinner suniv Family",
"Call trace: ",
" unwind_backtrace from show_stack+0x10/0x14",
" show_stack from dump_stack_lvl+0x38/0x48",
" dump_stack_lvl from panic+0xec/0x318",
" panic from mount_root_generic+0x228/0x29c",
" mount_root_generic from prepare_namespace+0x1a0/0x230",
" prepare_namespace from kernel_init+0x10/0x12c",
" kernel_init from ret_from_fork+0x14/0x28",
"Exception stack(0xc400dfb0 to 0xc400dff8)",
"dfa0:                                     00000000 00000000 00000000 00000000",
"dfc0: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000",
"dfe0: 00000000 00000000 00000000 00000000 00000013 00000000",
"---[ end Kernel panic - not syncing: VFS: Unable to mount root fs on '' or unknown-block(0,0) ]---",
};

void scroll_insert_line(wchar_t *buffer, wchar_t *new_line)
{
	if (line_num >= ROWS) {
		memmove(buffer, buffer + COLS, (ROWS - 1) * line_width);
		line_num = ROWS - 1;
	}

	offset = line_num * COLS;
	wcsncpy(buffer + offset, new_line, COLS);
	buffer[offset + COLS - 1] = L'\0';

	line_num++;
}

void console_wprintf_prefix(const wchar_t *format, ...)
{
	va_list args;
	wchar_t temp_buf[COLS + 1];
	wchar_t full_buf[COLS + 1];
	wchar_t time_prefix[TIME_PREFIX_LEN + 1];

	int now = time_us_32();
	int seconds = now / 1000000;
	int micros = now % 1000000;

	swprintf(time_prefix, TIME_PREFIX_LEN,
		L"[ %04d.%06d ] ", seconds, micros);

	va_start(args, format);
	vswprintf(temp_buf, COLS, format, args);

	wcsncpy(full_buf, time_prefix, TIME_PREFIX_LEN);
	wcsncat(full_buf, temp_buf, COLS - TIME_PREFIX_LEN);

	full_buf[COLS] = L'\0';

	scroll_insert_line(console_buf_font8x8, full_buf);

	va_end(args);
}

void console_wprintf(const wchar_t *format, ...)
{
	va_list args;
	wchar_t temp_buf[COLS + 1];

	va_start(args, format);
	vswprintf(temp_buf, COLS, format, args);

	temp_buf[COLS] = L'\0';

	scroll_insert_line(console_buf_font8x8, temp_buf);

	va_end(args);
}

#define console_printf(format, ...) \
	console_wprintf_prefix(L##format, __VA_ARGS__)

#define console_raw_printf(format, ...) \
	console_wprintf(L##format, __VA_ARGS__)

extern const unsigned char font6x9[];

int console_demo(hagl_backend_t *display)
{
	hagl_color_t colour = 0xffff;
	wmemset(console_buf_font8x8, L' ', ROWS * COLS);

	for (int i = 0; i < sizeof(logs)/sizeof(*logs); i++) {
		if (i == 0 || i == 1)
			console_raw_printf("%s\n", logs[i]);
		else
			console_printf("%s\n", logs[i]);
		printf("%s\n", logs[i]);
		for (int row = 0, y = 0; row < ROWS; row++, y+=8)
			hagl_put_text(display, &console_buf_font8x8[row * COLS], 0, y, colour, font6x9);
		hagl_flush(display);
	}
	return 0;
}
