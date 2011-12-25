#
# Copyright (C) 2011 Zyxmon
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=inadyn
PKG_VERSION:=1.98.0
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://keenetic.zyxmon.org/sources \
    ftp://ftp.vmlinux.org/pub/People/jocke/inadyn
PKG_MD5SUM:=158f01ee15f4fec03b490924295e721c

include $(INCLUDE_DIR)/package.mk

define Package/inadyn
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Inadyn is a free DynDNS  client
  URL:=http://vmlinux.org/jocke/inadyn.shtml
endef

define Package/inadyn/description
	inadyn - a client for open DNS servers.
endef

define Package/inadyn/conffiles
/etc/inadyn.conf
endef

define Package/inadyn/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/inadyn $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K11inadyn $(1)/etc/init.d/K11inadyn
	$(INSTALL_DATA) ./files/inadyn.conf $(1)/etc/inadyn.conf
endef

$(eval $(call BuildPackage,inadyn))
