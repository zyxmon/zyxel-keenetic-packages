#
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=nethelperd
PKG_VERSION:=0.01
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/nethelperd
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=nethelperd
	MAINTAINER:=Soul Trace <S-trace@list.ru>
	PRIORITY:=optional
endef

define Package/nethelperd/description
  nethelperd: Daemon to trace network activity of another process and run commands if it's appears or if no activity in some time.
  Used some parts from net-tools-1.60/netstat.c and from http://dixx.livejournal.com/12665.html
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-s -o $(PKG_BUILD_DIR)/nethelperd \
				$(PKG_BUILD_DIR)/nethelperd.c
endef

define Package/nethelperd/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nethelperd $(1)/bin/nethelperd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K87nethelper_webphp $(1)/etc/init.d/K87nethelper_webphp
endef

$(eval $(call BuildPackage,nethelperd))
