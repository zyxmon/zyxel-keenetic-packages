#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tesseract
PKG_VERSION:=3.01
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://tesseract-ocr.googlecode.com/files/
PKG_MD5SUM:=1ba496e51a42358fb9d3ffe781b2d20a

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/tesseract
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Tesseract Open Source OCR Engine
  URL:=http://tesseract-ocr.googlecode.com/
  DEPENDS:=+libleptonica +libstdcpp
endef

define Package/tesseract/description
	OCR
endef

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS += \
	LIBLEPT_HEADERSDIR="$(STAGING_DIR)/usr/include" \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--enable-shared=no

TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

define Build/Prepare
	$(call Build/Prepare/Default)
	$(SED) '1s/^\xEF\xBB\xBF//' $(PKG_BUILD_DIR)/ccutil/strngs.h 
endef

define Package/tesseract/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/share/tessdata/configs
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/share/tessdata/configs/* $(1)/usr/share/tessdata/configs/
	$(INSTALL_DIR) $(1)/usr/share/tessdata/tessconfigs
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/share/tessdata/tessconfigs/* $(1)/usr/share/tessdata/tessconfigs/
endef

$(eval $(call BuildPackage,tesseract))
