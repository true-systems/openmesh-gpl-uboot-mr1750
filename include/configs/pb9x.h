/*
 * Copyright (c) 2009, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */
/*
 * This file contains the configuration parameters for the pb93 board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/ar7240.h>
#define CONFIG_GPIO_CUSTOM      1
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS     1	    /* max number of memory banks */

#if (FLASH_SIZE == 16)
#define CFG_MAX_FLASH_SECT      64     /* max number of sectors on one chip */
#define CFG_FLASH_SECTOR_SIZE   (256*1024)
#define CFG_FLASH_SIZE          0x01000000 /* Total flash size */
#elif (FLASH_SIZE == 8)
#define CFG_MAX_FLASH_SECT      128    /* max number of sectors on one chip */
#define CFG_FLASH_SECTOR_SIZE   (64*1024)
#define CFG_FLASH_SIZE          0x00800000 /* Total flash size */
#else
#define CFG_MAX_FLASH_SECT      64     /* max number of sectors on one chip */
#define CFG_FLASH_SECTOR_SIZE   (64*1024)
#define CFG_FLASH_SIZE          0x00400000 /* Total flash size */
#endif

#define ENABLE_DYNAMIC_CONF 1
#define CONFIG_SUPPORT_AR7241 1

#define CONFIG_AR7242_S16_PHY
#define CONFIG_AG7240_GE0_IS_CONNECTED

#if (CFG_MAX_FLASH_SECT * CFG_FLASH_SECTOR_SIZE) != CFG_FLASH_SIZE
#	error "Invalid flash configuration"
#endif

#define CONFIG_SUPPORT_AR7241 1

#define CFG_FLASH_WORD_SIZE     unsigned short 

/* 
 * We boot from this flash
 */
#define CFG_FLASH_BASE		    0x9f000000

#ifdef COMPRESSED_UBOOT
#define BOOTSTRAP_TEXT_BASE         CFG_FLASH_BASE
#define BOOTSTRAP_CFG_MONITOR_BASE  BOOTSTRAP_TEXT_BASE
#endif

/*
 * Defines to change flash size on reboot
 */
#ifdef ENABLE_DYNAMIC_CONF
#define UBOOT_FLASH_SIZE          (256 * 1024)
#define UBOOT_ENV_SEC_START        (CFG_FLASH_BASE + UBOOT_FLASH_SIZE)

#define CFG_FLASH_MAGIC           0xaabacada
#define CFG_FLASH_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x20)
#define CFG_FLASH_SECTOR_SIZE_F   *(volatile int *)(CFG_FLASH_MAGIC_F + 0x4)
#define CFG_FLASH_SIZE_F          *(volatile int *)(CFG_FLASH_MAGIC_F + 0x8) /* Total flash size */
#define CFG_MAX_FLASH_SECT_F      (CFG_FLASH_SIZE / CFG_FLASH_SECTOR_SIZE) /* max number of sectors on one chip */
#endif

/* 
 * The following #defines are needed to get flash environment right 
 */
#define	CFG_MONITOR_BASE	TEXT_BASE
#define	CFG_MONITOR_LEN		(192 << 10)

/* default mtd partition table */
#undef MTDPARTS_DEFAULT
#ifdef COMPRESSED_UBOOT
#define MTDPARTS_DEFAULT    "mtdparts=ar7240-nor0:64k(u-boot),576k(kernel),1344k(rootfs),64k(ART)"
#else
#if (FLASH_SIZE == 16)
#define MTDPARTS_DEFAULT    "mtdparts=ar7240-nor0:256k(u-boot),256k(u-boot-env),1280k(custom),1536k(kernel),10752k(rootfs),640k(failsafe_k),1408k(failsafe_r),256k(ART)"
#elif (FLASH_SIZE == 8)
#define MTDPARTS_DEFAULT    "mtdparts=ar7240-nor0:256k(u-boot),64k(u-boot-env),320k(custom),1024k(kernel),4928k(rootfs),576k(failsafe_k),960k(failsafe_r),64k(ART)"
#else
#define MTDPARTS_DEFAULT    "mtdparts=ar7240-nor0:256k(u-boot),64k(u-boot-env),320k(custom),768k(kernel),2624k(rootfs),64k(ART)"
#endif
#endif /* #ifdef COMPRESSED_UBOOT */

#undef CONFIG_BOOTARGS
/* XXX - putting rootfs in last partition results in jffs errors */

#ifndef ROOTFS
#define ROOTFS 1
#endif

#ifdef COMPRESSED_UBOOT
#define	CONFIG_BOOTARGS     "console=ttyS0,115200 root=31:03 rootfstype=squashfs,jffs2 init=/etc/preinit "MTDPARTS_DEFAULT" board="BOARD_NAME
#else
#define	CONFIG_BOOTARGS     "console=ttyS0,115200 root=31:04 rootfstype=squashfs,jffs2 init=/etc/preinit "MTDPARTS_DEFAULT" board="BOARD_NAME
#endif /* #ifdef COMPRESSED_UBOOT */

#if (FLASH_SIZE == 16)
#define CONFIG_EXTRA_ENV_SETTINGS \
"imagechk=test -n \"${check_skip}\" || check_skip=1 && datachk vmlinux,rootfs\0" \
"bootcmd_0=tftp 0x81000000 vmlinux-initramfs.bin && bootm 0x81000000\0" \
"bootcmd_1=run imagechk && bootm 0x9f1c0000\0" \
"bootcmd_2=run imagechk && bootm 0x9fdc0000\0" \
"bootargs_2=console=ttyS0,115200 root=31:06 rootfstype=squashfs,jffs2 init=/etc/preinit "MTDPARTS_DEFAULT" board="BOARD_NAME"\0"
#elif (FLASH_SIZE == 8)
#define CONFIG_EXTRA_ENV_SETTINGS \
"imagechk=test -n \"${check_skip}\" || check_skip=1 && datachk vmlinux,rootfs\0" \
"bootcmd_0=tftp 0x81000000 vmlinux-initramfs.bin && bootm 0x81000000\0" \
"bootcmd_1=run imagechk && bootm 0x9f0a0000\0" \
"bootcmd_2=run imagechk && bootm 0x9f670000\0" \
"bootargs_2=console=ttyS0,115200 root=31:06 rootfstype=squashfs,jffs2 init=/etc/preinit "MTDPARTS_DEFAULT" board="BOARD_NAME"\0"
#else
#define CONFIG_EXTRA_ENV_SETTINGS \
"bootcmd_0=tftp 0x81000000 vmlinux-initramfs.bin && bootm 0x81000000\0" \
"bootcmd_1=bootm 0x9f0a0000\0"
#endif

#undef CFG_PLL_FREQ

#ifdef CONFIG_SUPPORT_AR7241
#define CFG_AR7241_PLL_FREQ     CFG_PLL_400_400_200
#endif

#define CFG_PLL_FREQ	CFG_PLL_400_400_200

#undef CFG_HZ

/*
 * MIPS32 24K Processor Core Family Software User's Manual
 *
 * 6.2.9 Count Register (CP0 Register 9, Select 0)
 * The Count register acts as a timer, incrementing at a constant
 * rate, whether or not an instruction is executed, retired, or
 * any forward progress is made through the pipeline.  The counter
 * increments every other clock, if the DC bit in the Cause register
 * is 0.
 */
/* Since the count is incremented every other tick, divide by 2 */
/* XXX derive this from CFG_PLL_FREQ */
#if (CFG_PLL_FREQ == CFG_PLL_200_200_100)
#   define CFG_HZ          (200000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_300_300_150)
#   define CFG_HZ          (300000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_350_350_175)
#   define CFG_HZ          (350000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_333_333_166)
#   define CFG_HZ          (333000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_133)
#   define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_66)
#   define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_400_400_200) || (CFG_PLL_FREQ == CFG_PLL_400_400_100)
#   define CFG_HZ          (400000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_320_320_80) || (CFG_PLL_FREQ == CFG_PLL_320_320_160)
#   define CFG_HZ          (320000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_410_400_200)
#   define CFG_HZ          (410000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_420_400_200)
#   define CFG_HZ          (420000000/2)
#endif

/*
 * timeout values are in ticks
 */
#define CFG_FLASH_ERASE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

/*
 * Cache lock for stack
 */
#define CFG_INIT_SP_OFFSET	0x1000

#ifndef COMPRESSED_UBOOT
#define	CFG_ENV_IS_IN_FLASH    1
#undef CFG_ENV_IS_NOWHERE
#else
#undef  CFG_ENV_IS_IN_FLASH
#define CFG_ENV_IS_NOWHERE  1
#endif /* #ifndef COMPRESSED_UBOOT */

/* Address and size of Primary Environment Sector	*/
#define CFG_ENV_ADDR		0x9f040000
#define CFG_ENV_SIZE            CFG_FLASH_SECTOR_SIZE 

#ifndef COMPRESSED_UBOOT
#if (FLASH_SIZE == 16) || (FLASH_SIZE == 8)
#define CONFIG_BOOTCOMMAND "test -n \"${preboot}\" && run preboot; test -n \"${bootseq}\" || bootseq=1,2; boot ${bootseq}"
#else
#define CONFIG_BOOTCOMMAND "test -n \"${preboot}\" && run preboot; test -n \"${bootseq}\" || bootseq=1; boot ${bootseq}"
#endif
#endif /* #ifndef COMPRESSED_UBOOT */

//#define CONFIG_FLASH_16BIT

/* DDR init values */

#define CONFIG_NR_DRAM_BANKS	2
#define CFG_DDR_REFRESH_VAL     0x4f10
#define CFG_DDR_CONFIG_VAL      0x7fbc8cd0
#define CFG_DDR_MODE_VAL_INIT   0x163
#define CFG_DDR_EXT_MODE_VAL    0x2
#define CFG_DDR_MODE_VAL        0x63

#define CFG_DDR_TRTW_VAL        0x1f
#define CFG_DDR_TWTR_VAL        0x1e

#define CFG_DDR_CONFIG2_VAL	 	0x99d0e6a8
#define CFG_DDR_RD_DATA_THIS_CYCLE_VAL  0x00ff

/* DDR values to support AR7241 */
#ifdef CONFIG_SUPPORT_AR7241 
#define CFG_7241_DDR1_CONFIG_VAL      0x7fbc8cd0
#define CFG_7241_DDR1_MODE_VAL_INIT   0x163
#define CFG_7241_DDR1_EXT_MODE_VAL    0x0
#define CFG_7241_DDR1_MODE_VAL        0x63
#define CFG_7241_DDR1_CONFIG2_VAL       0x99d0e6a8 

#define CFG_7241_DDR2_CONFIG_VAL        0x7fbc8cd0
#define CFG_7241_DDR2_MODE_VAL_INIT     0x163 
#define CFG_7241_DDR2_EXT_MODE_VAL      0x402
#define CFG_7241_DDR2_MODE_VAL          0x63
#define CFG_7241_DDR2_CONFIG2_VAL       0x99d0e6a8
#endif /* _SUPPORT_AR7241 */

/* DDR2 Init values */
#define CFG_DDR2_EXT_MODE_VAL    0x402

/* DDR value from Flash */
#ifdef ENABLE_DYNAMIC_CONF
#define CFG_DDR_MAGIC           0xaabacada
#define CFG_DDR_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x30)
#define CFG_DDR_CONFIG_VAL_F    *(volatile int *)(CFG_DDR_MAGIC_F + 4)
#define CFG_DDR_CONFIG2_VAL_F   *(volatile int *)(CFG_DDR_MAGIC_F + 8)
#define CFG_DDR_EXT_MODE_VAL_F  *(volatile int *)(CFG_DDR_MAGIC_F + 12)
#endif

#define CONFIG_NET_MULTI

#define CONFIG_MEMSIZE_IN_BYTES
#ifndef COMPRESSED_UBOOT
#define CONFIG_PCI 1
#endif /* #ifndef COMPRESSED_UBOOT */

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#ifndef COMPRESSED_UBOOT
#define CONFIG_COMMANDS	(( CONFIG_CMD_DFL | CFG_CMD_DHCP | CFG_CMD_ELF | CFG_CMD_PCI |	\
	CFG_CMD_MII | CFG_CMD_PING | CFG_CMD_NET | CFG_CMD_ENV | CFG_CMD_PLL | \
	CFG_CMD_FLASH | CFG_CMD_LOADS | CFG_CMD_RUN | CFG_CMD_LOADB | CFG_CMD_ELF | CFG_CMD_DDR | CFG_CMD_ETHREG))
#else
#define CONFIG_COMMANDS (( CONFIG_CMD_DFL | CFG_CMD_PING | CFG_CMD_NET))
#endif /* #ifndef COMPRESSED_UBOOT */

#define CONFIG_IPADDR   192.168.1.1
#define CONFIG_SERVERIP 192.168.1.101
#define CONFIG_ETHADDR 0x00:0xaa:0xbb:0xcc:0xdd:0xee
#define CONFIG_ENV_OVERWRITE        1
#define CFG_FAULT_ECHO_LINK_DOWN    1


#define CFG_PHY_ADDR 0 
#define CFG_AG7240_NMACS 1
#define CFG_GMII     0
#define CFG_MII0_RMII             1
#define CFG_AG7100_GE0_RMII             1

#define CFG_BOOTM_LEN	(16 << 20) /* 16 MB */
#define DEBUG
#define CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2 "hush>"

/*
** Parameters defining the location of the calibration/initialization
** information for the two Merlin devices.
** NOTE: **This will change with different flash configurations**
*/

#if (FLASH_SIZE == 16)
#define WLANCAL                         0xbffc1000
#define BOARDCAL                        0xbffc0000
#elif (FLASH_SIZE == 8)
#define WLANCAL                         0xbf7f1000
#define BOARDCAL                        0xbf7f0000
#else
#define WLANCAL                         0xbf3f1000
#define BOARDCAL                        0xbf3f0000
#endif
#define ATHEROS_PRODUCT_ID              137
#define CAL_SECTOR                      (CFG_MAX_FLASH_SECT - 1)

/* For Merlin, both PCI, PCI-E interfaces are valid */
#define AR7240_ART_PCICFG_OFFSET        12

#include <cmd_confdefs.h>

#endif	/* __CONFIG_H */
