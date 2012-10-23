include $(TOPDIR)/rules.mk

PKG_NAME:=Automatic
PKG_VERSION:=0.8.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/1100101-$(PKG_NAME)-ae2ed5f/
PKG_SOURCE_URL:=https://github.com/1100101/Automatic/zipball/v$(PKG_VERSION)
PKG_SOURCE:=1100101-$(PKG_NAME)-v$(PKG_VERSION)-0-g95ada5c.zip
PKG_MD5SUM:=c3fc1b16586940ad120b0c241b08a8ed

PKG_BUILD_DEPENDS:=libcurl
PKG_FIXUP:=autoreconf
include $(INCLUDE_DIR)/package.mk

define Package/automatic
	SECTION:=utils
	CATEGORY:=Utilities
	DEPENDS:=+libpcre +libxml2
	TITLE:=Automatic is an RSS downloader for Linux based systems, that grabs your favorite torrents based on filters you specify.
	URL:=http://kylek.is-a-geek.org:31337/Automatic/
endef

define Package/automatic/description
	Automatic is an RSS downloader for Linux based systems, that grabs your favorite torrents based on filters you specify.
endef

define Package/aria2/conffiles
/etc/automatic.conf
endef

CONFIGURE_VARS += \
	LDFLAGS="$$$$LDFLAGS -lcrypto -lssl -lz"

define Package/automatic/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/automatic $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/src/automatic.conf-sample $(1)/etc/automatic.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K91automatic $(1)/etc/init.d/K91automatic
endef

$(eval $(call BuildPackage,automatic))

