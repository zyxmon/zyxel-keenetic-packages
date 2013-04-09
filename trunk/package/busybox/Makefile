# 
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=busybox
PKG_VERSION:=1.19.4
PKG_RELEASE:=4
#PKG_FLAGS:=essential

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.busybox.net/downloads \
		http://distfiles.gentoo.org/distfiles/
PKG_MD5SUM:=9c0cae5a0379228e7b55e5b29528df8e

PKG_BUILD_PARALLEL:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

ifeq ($(BUILD_VARIANT),storage)
  PATCH_DIR:=spatches
endif

include $(INCLUDE_DIR)/package.mk

ifeq ($(DUMP),)
  STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell $(SH_FUNC) grep '^CONFIG_BUSYBOX_' $(TOPDIR)/.config | md5s)
endif

init-y :=
init-$(CONFIG_BUSYBOX_CONFIG_HTTPD) += httpd
init-$(CONFIG_BUSYBOX_CONFIG_CROND) += cron
init-$(CONFIG_BUSYBOX_CONFIG_TELNETD) += telnet

define Package/busybox/Default
  SECTION:=base
  CATEGORY:=Base system
  MAINTAINER:=Nicolas Thill <nico@openwrt.org>
  TITLE:=Core utilities for embedded Linux
  URL:=http://busybox.net/
  MENU:=1
endef

define Package/busybox
  $(call Package/busybox/Default)
  DEPENDS:=@TARGET_keenetic
  VARIANT:=usb
endef

define Package/busybox-storage
  $(call Package/busybox/Default)
  DEPENDS:=@TARGET_keenetic_storage
  VARIANT:=storage
endef

define Package/busybox/Default/description
 The Swiss Army Knife of embedded Linux.
 It slices, it dices, it makes Julian Fries.
endef

define Package/busybox/description
  $(call Package/busybox/Default/description)
  This package is built for usb.
endef

define Package/busybox-storage/description
  $(call Package/busybox/Default/description)
  This package is built for internal storage/
endef

define Package/busybox/config
	source "$(SOURCE)/Config.in"
endef

define Package/busybox-storage/config
	source "$(SOURCE)/Config-storage.in"
endef

define Build/Configure
	rm -f $(PKG_BUILD_DIR)/.configured*
	grep 'CONFIG_BUSYBOX_' $(TOPDIR)/.config | sed -e "s,\\(# \)\\?CONFIG_BUSYBOX_\\(.*\\),\\1\\2,g" > $(PKG_BUILD_DIR)/.config
	yes '' | $(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		KBUILD_HAVE_NLS=no \
		ARCH="$(ARCH)" \
		oldconfig
endef

ifdef CONFIG_GCC_VERSION_LLVM
  TARGET_CFLAGS += -fnested-functions
endif

LDLIBS:=m crypt

define Build/Compile
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		KBUILD_HAVE_NLS=no \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		ARCH="$(ARCH)" \
		SKIP_STRIP=y \
		LDLIBS="$(LDLIBS)" \
		all
	rm -rf $(PKG_INSTALL_DIR)
	$(FIND) $(PKG_BUILD_DIR) -lname "*busybox" -exec rm \{\} \;
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		ARCH="$(ARCH)" \
		CONFIG_PREFIX="$(PKG_INSTALL_DIR)" \
		LDLIBS="$(LDLIBS)" \
		install
endef

define Package/busybox/conffiles
/etc/crontabs/root
endef



define Package/busybox/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K02cron $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/crontabs
	$(INSTALL_DATA) ./files/crontab $(1)/etc/crontabs/root
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
	-rm -rf $(1)/lib64
endef

define Package/busybox-storage/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K02cron-storage $(1)/etc/init.d/K02cron
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) ./files/ext_init.sh $(1)/bin/
	$(INSTALL_BIN) ./files/ext_firewall.sh $(1)/bin/
	$(INSTALL_DIR) $(1)/etc/crontabs
	$(INSTALL_DATA) ./files/crontab $(1)/etc/crontabs/root
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
	-rm -rf $(1)/lib64
endef

$(eval $(call BuildPackage,busybox))
$(eval $(call BuildPackage,busybox-storage))
