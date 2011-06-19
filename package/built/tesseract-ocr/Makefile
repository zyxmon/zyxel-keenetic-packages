include $(TOPDIR)/rules.mk

PKG_NAME:=tesseract
PKG_VERSION:=3.00
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://tesseract-ocr.googlecode.com/files/
PKG_MD5SUM:= cc812a261088ea0c3d2da735be35d09f
PKG_INSTALL:=1
PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk

define Package/tesseract
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=tesseract
	DEPENDS:=+libtiff +libstdcpp +libjpeg +libpng
	URL:=http://tesseract-ocr.googlecode.com/files/
endef

define Package/tesseract/description
	OCR
endef

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS += \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--enable-shared=no

define Package/tesseract/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/share/tessdata/configs
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/share/tessdata/configs/* $(1)/usr/share/tessdata/configs/
	$(INSTALL_DIR) $(1)/usr/share/tessdata/tessconfigs
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/share/tessdata/tessconfigs/* $(1)/usr/share/tessdata/tessconfigs/
endef

$(eval $(call BuildPackage,tesseract))
