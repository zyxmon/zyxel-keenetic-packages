# 
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pptpd
PKG_VERSION:=1.3.4
PKG_RELEASE:=4

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/poptop
PKG_MD5SUM:=b38df9c431041922c997c1148bedf591

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/pptpd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=PopTop pptp server
  URL:=http://www.poptop.org/
  SUBMENU:=VPN
endef

CONFIGURE_ARGS += \
	--enable-bcrelay \

CONFIGURE_VARS += \
	ac_cv_header_libintl_h=no \

MAKE_FLAGS += \
	COPTS="$(TARGET_CFLAGS)" \
	INSTALL="install" \

define Package/pptpd/conffiles
/etc/pptpd.conf
/etc/ppp/options.pptpd
/etc/ppp/chap-secrets
endef

define Package/pptpd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/sbin/bcrelay \
		$(PKG_INSTALL_DIR)/usr/sbin/pptpctrl \
		$(PKG_INSTALL_DIR)/usr/sbin/pptpd \
		$(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib/pptpd
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pptpd/* $(1)/usr/lib/pptpd/
	$(INSTALL_DIR) $(1)/etc/ppp
	$(INSTALL_CONF) ./files/pptpd.conf $(1)/etc/
	$(INSTALL_CONF) ./files/options.pptpd $(1)/etc/ppp/options.pptpd
	$(INSTALL_CONF) ./files/chap-secrets $(1)/etc/ppp/chap-secrets
	$(INSTALL_BIN) ./files/ip-up $(1)/etc/ppp/
	$(INSTALL_BIN) ./files/ip-down $(1)/etc/ppp/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K60pptpd $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/firewall.d
	$(INSTALL_BIN) ./files/iptables_pptpd.sh $(1)/etc/firewall.d/
endef

$(eval $(call BuildPackage,pptpd))
