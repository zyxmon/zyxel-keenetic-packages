#
# Copyright (C) 2011 ZyXEL.ru
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=usb_ups_apc_smart
PKG_VERSION:=1.0.5
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:= libusb

include $(INCLUDE_DIR)/package.mk

define Package/usb_ups_apc_smart
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=USB UPS APC Smart-UPS
endef

define Package/usb_ups_apc_smart/description
  USB UPS APC Smart-UPS monitor.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-L $(STAGING_DIR)/usr/lib/ -lusb \
		-s -o $(PKG_BUILD_DIR)/usb_ups_apc_smart \
				$(PKG_BUILD_DIR)/usb_ups_apc_smart.c
endef

define Package/usb_ups_apc_smart/install	
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/usb_ups_apc_smart $(1)/bin/usb_ups_apc_smart
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K98ups $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,usb_ups_apc_smart))
