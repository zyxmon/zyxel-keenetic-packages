#
# Copyright (C) 2009-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=git
PKG_VERSION:=1.8.3.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://git-core.googlecode.com/files/
PKG_MD5SUM:=83b792fd06066c77e1d4911ebba2d701

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/git
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Version Control Systems
  DEPENDS:=$(ICONV_DEPENDS) $(INTL_DEPENDS) +libcurl +libexpat
  TITLE:=The fast version control system
  URL:=http://git-scm.com
endef

define Package/git/description
 Git is a free & open source, distributed version control system
 designed to handle everything from small to very large projects
 with speed and efficiency.
endef

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS += \
	--with-iconv="$(ICONV_PREFIX)" \
	--with-gitconfig="$(ROOT_ZYX)/etc/gitconfig" \
	--with-gitattributes="$(ROOT_ZYX)/etc/gitattributes" \
	--without-python \
	--without-tcltk \

CONFIGURE_VARS += \
	ac_cv_fread_reads_directories=no \
	ac_cv_snprintf_returns_bogus=no

TARGET_LDFLAGS += \
	-Wl,-rpath,$(PREFIX_ZYX)/lib

MAKE_FLAGS += \
	NO_INSTALL_HARDLINKS="Yes" \
	NO_PERL="Yes" \
	NEEDS_SSL_WITH_CURL="Yes" \
	NEEDS_CRYPTO_WITH_SSL="Yes" \
	NO_PYTHON="Yes" \
	NO_TCLTK="Yes"
	

define Package/git/install
	$(INSTALL_DIR) $(1)
	for app in git git-cvsserver git-shell git-upload-pack; do \
		$(RM) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/bin/$$$${app} ; \
		$(LN) -s $(PREFIX_ZYX)/lib/git-core/$$$${app} $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/bin/$$$${app} ; \
	done
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/* $(1)/
endef

$(eval $(call BuildPackage,git))
