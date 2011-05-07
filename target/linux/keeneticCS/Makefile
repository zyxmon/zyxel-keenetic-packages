# 
# Copyright (C) 2010 ZyXEL.ru
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mipsel
BOARD:=keeneticCS
BOARDNAME:=ZyXEL Keenetic CodeSourcery
FEATURES:=squashfs
CFLAGS:=-Os -pipe -mips32r2 -mtune=mips32r2 -funit-at-a-time

LINUX_VERSION:=2.6.23-rt
UCLIBC_VERSION:=0.9.30.3

# include the profiles
include $(INCLUDE_DIR)/target.mk

define Target/Description
	Build firmware image for ZyXEL Keenetic
endef

$(eval $(call BuildTarget))
