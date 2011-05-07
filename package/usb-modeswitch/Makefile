#
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=usb_modeswitch
PKG_VERSION:=1.1.7
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/usb-modeswitch
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libusb
  TITLE:=USB ModeSwitch
endef

define Package/usb-modeswitch/description
  A mode switching tool for controlling
  "flip flop" (multiple device) USB gear.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-L $(STAGING_DIR)/usr/lib/ -lusb \
		-s -o $(PKG_BUILD_DIR)/usb_modeswitch \
				$(PKG_BUILD_DIR)/usb_modeswitch.c
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-L $(STAGING_DIR)/usr/lib/ -lusb \
		-s -o $(PKG_BUILD_DIR)/old_usb_modeswitch \
				$(PKG_BUILD_DIR)/old_usb_modeswitch.c
endef

define Package/usb-modeswitch/install	
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/usb_modeswitch $(1)/bin/usb_modeswitch
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/old_usb_modeswitch $(1)/bin/old_usb_modeswitch
endef

$(eval $(call BuildPackage,usb-modeswitch))
