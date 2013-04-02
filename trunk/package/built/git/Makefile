#
# Copyright (C) 2009-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=git
PKG_VERSION:=1.7.11.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://git-core.googlecode.com/files/
PKG_MD5SUM:=91e01a22032f5d0ca2941ae7fd49d587

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=libopenssl

include $(INCLUDE_DIR)/package.mk

define Package/git
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Version Control Systems
  TITLE:=The fast version control system
  URL:=http://git-scm.com
endef

define Package/git/description
 Git is a free & open source, distributed version control system
 designed to handle everything from small to very large projects
 with speed and efficiency.
endef

MAKE_FLAGS := \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	CPPFLAGS="$(TARGET_CPPFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	NO_CURL="YesPlease" \
	NO_EXPAT="YesPlease" \
	NO_MKSTEMPS="YesPlease" \
	NO_GETTEXT="YesPlease" \
	NO_UNIX_SOCKETS="YesPlease" \
	NO_IPV6="YesPlease" \
	NO_ICONV="YesPlease" \
	NO_NSEC="YesPlease" \
	NO_PERL="YesPlease" \
	NO_PYTHON="YesPlease" \
	NO_TCLTK="YesPlease"

define Build/Configure
	$(MAKE) -C $(PKG_BUILD_DIR) \
		configure

	( cd $(PKG_BUILD_DIR); \
		./configure --prefix="$(PREFIX_ZYX)" \
	);
endef

define Package/git/install
	$(INSTALL_DIR) $(1)
	$(RM) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/bin/git-cvsserver
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/* $(1)/
	ln -s $(PREFIX_ZYX)/bin/git $(1)/usr/libexec/git-core/git
endef

$(eval $(call BuildPackage,git))
