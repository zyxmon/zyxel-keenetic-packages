#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=psmisc
PKG_VERSION:=22.16
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/psmisc
PKG_MD5SUM:=d52caf26159c905574c2ce4bd792b9ee

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/psmisc
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libncurses
  TITLE:=proc utilities
  URL:=http://psmisc.sourceforge.net/
endef

define Package/psmisc/description
 psmisc is a set of additional small useful utilities that use
 the proc filesystem: fuser, killall, pstree, and prtstat
endef

CONFIGURE_ARGS += \
	--disable-harden-flags

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS)" \
	CPPFLAGS="$(TARGET_CPPFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)"

define Package/psmisc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/{fuser,prtstat,pstree} \
		$(1)/usr/bin/
endef

$(eval $(call BuildPackage,psmisc))
