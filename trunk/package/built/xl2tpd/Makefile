#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xl2tpd
PKG_VERSION:=1.3.1
PKG_RELEASE:=7

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
# Host seems to be down.
#PKG_SOURCE_URL:=ftp://ftp.xelerance.com/xl2tpd/
PKG_MD5SUM:=cf61576fef5c2d6c68279a408ec1f0d5
PKG_SOURCE_URL:=http://pkgs.fedoraproject.org/lookaside/pkgs/xl2tpd/xl2tpd-$(PKG_VERSION).tar.gz/$(PKG_MD5SUM)/

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/xl2tpd
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+ppp
  TITLE:=An L2TP (Layer 2 Tunneling Protocol) daemon
  URL:=http://www.xelerance.com/software/xl2tpd/
  SUBMENU:=VPN
endef

define Package/xl2tpd/description
l2tpd is the open source implementation of the L2TP tunneling protocol (RFC2661).
It does implement both LAC and LNS role in a L2TP networking architecture. The
main goal of this protocol is to tunnel PPP frame trough an IP network.
endef

# XXX: CFLAGS are already set by Build/Compile/Default
MAKE_FLAGS+= \
	OFLAGS=""

define Package/xl2tpd/conffiles
/etc/xl2tpd/xl2tpd.conf
/etc/xl2tpd/xl2tp-secrets
/etc/ppp/options.xl2tpd
/etc/ppp/chap-secrets
/etc/ppp/ip-up.sh
/etc/ppp/ip-down.sh
endef

define Package/xl2tpd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/local/sbin/xl2tpd $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/local/sbin/xl2tpd-control $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K58xl2tpd $(1)/etc/init.d/

	$(INSTALL_DIR) $(1)/etc/firewall.d
	$(INSTALL_BIN) ./files/iptables_xl2tpd.sh $(1)/etc/firewall.d/

	$(INSTALL_DIR) $(1)/etc/xl2tpd
	$(INSTALL_DATA) ./files/xl2tpd.conf $(1)/etc/xl2tpd/
	$(INSTALL_CONF) ./files/xl2tp-secrets $(1)/etc/xl2tpd/

	$(INSTALL_DIR) $(1)/etc/ppp
	$(INSTALL_CONF) ./files/chap-secrets $(1)/etc/ppp/chap-secrets
	$(INSTALL_DATA) ./files/options.xl2tpd $(1)/etc/ppp/options.xl2tpd
	$(INSTALL_BIN) ./files/ip-up.sh $(1)/etc/ppp/
	$(INSTALL_BIN) ./files/ip-down.sh $(1)/etc/ppp/
	$(INSTALL_DIR) $(1)/var/run
endef

$(eval $(call BuildPackage,xl2tpd))
