# 
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mc
PKG_VERSION:=4.8.8
PKG_RELEASE:=1
PKG_INSTALL:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.midnight-commander.org/downloads/
PKG_MD5SUM:=324ff5a192d30d3a3b234c130550eb0a

#PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/mc
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=$(ICONV_DEPENDS) $(INTL_DEPENDS) +glib2 +libncurses +libslang2
  TITLE:=midnight commander - a powerful file manager
  URL:=http://www.midnight-commander.org/
  SUBMENU:=filemanager
endef

define Package/mc/description
	GNU Midnight Commander is a text-mode full-screen file manager. 
	It uses a two panel interface and a subshell for command execution. 
	It includes an internal editor with syntax highlighting and an 
	internal viewer with support for binary files. Also included is 
	Virtual Filesystem (VFS), that allows files on remote systems 
	(e.g. FTP, SSH, SMB servers) and files inside archives to be 
	manipulated like real files.
endef

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS += \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--without-gpm-mouse \
	--without-x \
	--disable-glibtest \
	--disable-doxygen-doc \
	--disable-nls \
	--disable-tests \
	--with-libintl-prefix="$(INTL_PREFIX)" \
	--with-libiconv-prefix="$(ICONV_PREFIX)" \
	--with-slang-includes="$(STAGING_DIR)/usr/include" \
	--with-slang-libs="$(STAGING_DIR)/usr/lib" \
	fu_cv_sys_stat_statfs2_bsize=yes


AM_HOST=$(firstword $(wildcard $(STAGING_DIR_HOST)/share/automake-*))

define Build/Prepare
	$(Build/Prepare/Default)
	for script in config.guess config.sub depcomp install-sh missing; do \
		rm -f $(PKG_BUILD_DIR)/config/$$$$script; \
		ln -s $(AM_HOST)/$$$$script $(PKG_BUILD_DIR)/config/$$$$script; \
	done
endef

define Package/mc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/bin/mc $(1)/usr/bin/
	ln -nsf mc $(1)/usr/bin/mcedit
	ln -nsf mc $(1)/usr/bin/mcdiff
	ln -nsf mc $(1)/usr/bin/mcview
	$(INSTALL_DIR) $(1)/etc/mc
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/etc/mc/* $(1)/etc/mc/
	$(INSTALL_DIR) $(1)/usr/lib/mc
	$(INSTALL_DIR) $(1)/usr/lib/mc/extfs.d
	$(INSTALL_DIR) $(1)/usr/lib/mc/fish
	$(CP) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/lib/mc/* $(1)/usr/lib/mc/
	$(INSTALL_DIR) $(1)/usr/share/mc
	$(INSTALL_DIR) $(1)/usr/share/mc/help
	$(INSTALL_DIR) $(1)/usr/share/mc/skins
	$(INSTALL_DIR) $(1)/usr/share/mc/hints
	$(INSTALL_DIR) $(1)/usr/share/mc/syntax
	$(CP) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/share/mc/* $(1)/usr/share/mc/
endef

$(eval $(call BuildPackage,mc))
