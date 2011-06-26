include $(TOPDIR)/rules.mk

PKG_NAME:=aria2
PKG_VERSION:=1.11.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/aria2


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)



include $(INCLUDE_DIR)/package.mk


define Package/aria2
SECTION:=net
CATEGORY:=Network
TITLE:=download manager
URL:=http://aria2.sourceforge.net/
DEPENDS:=+libxml2 +libcares +libsqlite3 +libstdcpp
endef

define Package/aria2/description
	download manager
endef

CONFIGURE_ARGS += \
	--with-openssl-prefix="$(STAGING_DIR)/usr/include/openssl" \
	--with-libz-prefix="$(STAGING_DIR)/usr/include" \
	--without-gnutls \
	--without-libexpat \
	--with-openssl \
	--with-libxml2 \
	--with-libcares \
	--disable-nls \
	--disable-static \
	--oldincludedir=$(PKG_BUILD_DIR)/.




define Package/aria2/install
	$(INSTALL_DIR) -d $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/aria2c $(1)/usr/bin/
endef

$(eval $(call BuildPackage,aria2))
