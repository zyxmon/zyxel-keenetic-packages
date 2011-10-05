#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sane-backends
PKG_VERSION:=1.0.22
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=ftp://ftp.sane-project.org/pub/sane/$(PKG_NAME)-$(PKG_VERSION) \
	https://alioth.debian.org/frs/download.php/3503
PKG_MD5SUM:=fadf56a60f4776bfb24491f66b617cf5

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/sane-backends/Default
  TITLE:=Scanner Access Now Easy
  URL:=http://www.sane-project.org/
endef

define Package/sane-backends/Default/description
 SANE (Scanner Access Now Easy) is a universal scanner interface.
endef

define Package/sane-backends
$(call Package/sane-backends/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+sane-libs +libtiff
  TITLE+= (backends)
endef

define Package/sane-backends/description
$(call Package/sane-backends/Default/description)
 This package contains the SANE daemon.
endef

define Package/sane-libs
  $(call Package/sane-backends/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libjpeg
  TITLE+= (libraries)
endef

define Package/sane-libs/description
$(call Package/sane-backends/Default/description)
 This package contains the SANE shared libraries.
endef

define Package/sane-frontends
  $(call Package/sane-backends/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+sane-backends
  TITLE+= (frontends)
endef

define Package/sane-frontends/description
$(call Package/sane-backends/Default/description)
 This package contains the SANE frontends.
endef

TARGET_LDFLAGS += \
	-Wl,-rpath,$(PREFIX_ZYX)/lib/sane \
	-Wl,-rpath-link $(STAGING_DIR)/usr/lib \
	-Wl,-rpath-link $(PKG_BUILD_DIR)/backend/.libs
	

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS+= \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--enable-shared \
	--enable-static \
	--disable-translations \
	--disable-latex \
	--without-gphoto2 \
	--disable-debug \
	--without-snmp

define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/backend/.libs
	$(Build/Configure/Default)
endef

define Package/sane-backends/conffiles
/etc/sane.d/saned.conf
/etc/sane.d/dll.conf
endef

define Package/sane-backends/install
	$(INSTALL_DIR) $(1)/etc/sane.d
	$(INSTALL_CONF) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/etc/sane.d/*.conf $(1)/etc/sane.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/sbin/* $(1)/usr/sbin/
endef

define Package/sane-libs/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/lib/libsane.so.* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/sane
	$(CP) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/lib/sane/libsane-*.so.* $(1)/usr/lib/sane/
endef

define Package/sane-frontends/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/bin/* $(1)/usr/bin/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/include/sane $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/lib/libsane.{a,so*} $(1)/usr/lib/
endef

$(eval $(call BuildPackage,sane-backends))
$(eval $(call BuildPackage,sane-libs))
$(eval $(call BuildPackage,sane-frontends))
