# 
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=3proxy
PKG_VERSION:=0.6.1
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tgz
PKG_SOURCE_URL:=http://www.3proxy.ru/0.6.1
PKG_MD5SUM:=03790467a12e16ed8785ae688ecf9b68

include $(INCLUDE_DIR)/package.mk

define Package/3proxy
  SUBMENU:=Web Servers/Proxies
  SECTION:=net
  CATEGORY:=Network
  TITLE:=3proxy free proxy server
  URL:=http://www.3proxy.ru/
endef

define Package/3proxy/description
  3proxy is universal proxy server. It can be used to provide internal users with fully controllable 
  access to external resources or to provide external users with access to internal resources.
endef

define Package/3proxy/conffiles
	/etc/3proxy/3proxy.cfg
	/etc/3proxy/bandlimiters
	/etc/3proxy/counters
	/etc/3proxy/passwd
endef

define Build/Configure
	$(CP) ./files/Makefile $(PKG_BUILD_DIR)/Makefile
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR=$(STAGING_DIR) \
		CC="$(TARGET_CC)" \
		LN="$(TARGET_CC)"

endef


define Package/3proxy/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/3proxy $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/countersutil $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/dighosts $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ftppr $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/mycrypt $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/pop3p $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/proxy $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/socks $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/tcppm $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/udppm $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/3proxy
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_CONF) ./files/3proxy.cfg $(1)/etc/3proxy/
	$(INSTALL_BIN) ./files/add3proxyuser.sh $(1)/etc/3proxy/
	$(INSTALL_DIR) $(1)/var/log/3proxy
	$(INSTALL_BIN) ./files/K68_3proxy $(1)/etc/init.d
endef


$(eval $(call BuildPackage,3proxy))

