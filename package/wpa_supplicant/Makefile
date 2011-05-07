# 
# Copyright (C) 2006 OpenWrt.org
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wpa_supplicant
PKG_VERSION:=0.5.9
PKG_RELEASE:=2
PKG_MD5SUM:=390e46066fa96ef9ba8e2dd63fca5aa0

PKG_SOURCE_URL:=http://downloads.openwrt.org/sources/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define Package/wpa-supplicant
  SECTION:=net
  CATEGORY:=Network
  TITLE:=WPA Supplicant
  MENU:=1
ifeq ($(CONFIG_WPASUPP_OPENSSL), y)
  DEPENDS:=+libopenssl
endif
  URL:=http://hostap.epitest.fi/wpa_supplicant/
endef

define Package/wpa-supplicant/config
	menu "wpa-supplicant options"
	    depends on PACKAGE_wpa-supplicant
	    source "$(SOURCE)/Config.in"
	endmenu
endef

define Package/wpa-supplicant/Description
 WPA Supplicant
endef

define Package/wpa-cli
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=wpa-supplicant
  TITLE:=WPA Supplicant command line interface
endef

define Package/wpa-cli/Description
 
endef


define Build/Configure
	cp ./files/config.$(ARCH) $(PKG_BUILD_DIR)/.config
ifeq ($(CONFIG_WPASUPP_RALINK), y)
	echo "CONFIG_DRIVER_RALINK=y" >> $(PKG_BUILD_DIR)/.config
endif
ifneq ($(CONFIG_WPASUPP_OPENSSL), y)
	echo "CONFIG_TLS=internal" >> $(PKG_BUILD_DIR)/.config
	echo "CONFIG_INTERNAL_LIBTOMMATH=1" >> $(PKG_BUILD_DIR)/.config
endif
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		KERNEL=$(LINUX_DIR) \
		CPPFLAGS="$(TARGET_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		all
	$(STRIP) $(PKG_BUILD_DIR)/wpa_supplicant
	$(STRIP) $(PKG_BUILD_DIR)/wpa_cli
		
endef

define Package/wpa-supplicant/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/wpa_supplicant $(1)/usr/sbin/
endef

define Package/wpa-cli/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/wpa_{cli,passphrase} $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,wpa-supplicant))
$(eval $(call BuildPackage,wpa-cli))
