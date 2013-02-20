#
# Copyright (C) 2011-2012 Entware
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=microdc2
PKG_VERSION:=0.15.6-20130122
PKG_RELEASE:=1

PKG_REV:=95860723598d547267e9372f12306d4f52a75bca
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://github.com/jnwatts/microdc2.git
PKG_SOURCE_VERSION:=$(PKG_REV)

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/microdc2
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=P2P
	DEPENDS:=$(ICONV_DEPENDS) +libxml2 +libreadline +libncurses
	TITLE:=microdc2
	URL:=https://github.com/jnwatts/microdc2
	MAINTAINER:=Entware team, wl500g-repo.googlecode.com
endef

define Package/microdc2/description
	A command-line based Direct Connect client.
endef

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS += \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--with-libiconv-prefix="$(ICONV_PREFIX)" \
	--with-libintl-prefix="$(INTL_PREFIX)" \
	--disable-rpath

TARGET_LDFLAGS += -lz

define Package/microdc2/conffiles
/etc/microdc2/config
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LTLIBICONV="" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		install
endef

define Package/microdc2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/microdc2
	$(INSTALL_CONF) ./files/microdc2.conf $(1)/etc/microdc2/config
endef

$(eval $(call BuildPackage,microdc2))
