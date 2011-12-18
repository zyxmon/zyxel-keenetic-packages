include $(TOPDIR)/rules.mk

PKG_NAME:=eng.traineddata
PKG_VERSION:=3.01
PKG_RELEASE:=1

PKG_SOURCE:=tesseract-ocr-3.01.eng.tar.gz
PKG_SOURCE_URL:=http://tesseract-ocr.googlecode.com/files/
PKG_MD5SUM:=89c139a73e0e7b1225809fc7b226b6c9
PKG_BUILD_DIR:=$(BUILD_DIR)/tessract-ocr-eng-data

PKG_UNPACK:=cp $(DL_DIR)/$(PKG_SOURCE) $(PKG_BUILD_DIR)

include $(INCLUDE_DIR)/package.mk

define Package/tesseract-eng-data
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=tesseract-eng-data
	DEPENDS:=+tesseract
	URL:=http://tesseract-ocr.googlecode.com/files/
endef

define Package/tesseract-eng-data/description
	English data file for tesseract3
endef

define Package/tesseract-eng-data/Prepare
$(call Build/Prepare/Default)
endef

define Build/Configure
endef

define Build/Compile
#	gunzip -d $(PKG_BUILD_DIR)/$(PKG_SOURCE)
	$(TAR) -C $(PKG_BUILD_DIR) -xvzf $(PKG_BUILD_DIR)/$(PKG_SOURCE)
endef


define Package/tesseract-eng-data/install
	$(INSTALL_DIR) $(1)/usr/share/tessdata
	$(CP) $(PKG_BUILD_DIR)/tesseract-ocr/tessdata/* $(1)/usr/share/tessdata/
endef

$(eval $(call BuildPackage,tesseract-eng-data))
