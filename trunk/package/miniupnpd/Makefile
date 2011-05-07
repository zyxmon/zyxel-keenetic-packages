#
# Copyright (C) 2006 OpenWrt.org
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=miniupnpd
PKG_VERSION:=1.4.20100921
PKG_RELEASE:=2
PKG_MD5SUM:=8840fefae59ad3859f9212dcd780d3bf

PKG_SOURCE_URL:=http://miniupnp.free.fr/files
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/miniupnpd
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+iptables
  TITLE:=Lightweight UPnP daemon
  URL:=http://miniupnp.tuxfamily.org/
endef

define Build/Compile
	ln -sf $(PKG_BUILD_DIR)/config.h.openwrt $(PKG_BUILD_DIR)/config.h
	$(MAKE) -C $(PKG_BUILD_DIR) -f Makefile.openwrt \
		$(TARGET_CONFIGURE_OPTS) \
		STAGING_DIR="$(STAGING_DIR)" \
		CFLAGS="$(TARGET_CFLAGS) -I $(STAGING_DIR)/usr/include" \
		LIBS="$(STAGING_DIR)/usr/lib/libiptc.a"
endef

define Package/miniupnpd/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/miniupnpd $(1)/usr/bin/
	$(INSTALL_DATA) ./files/miniupnpd.conf $(1)/etc/miniupnpd.conf
endef

$(eval $(call BuildPackage,miniupnpd))
