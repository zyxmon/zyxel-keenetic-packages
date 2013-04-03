include $(TOPDIR)/rules.mk

PKG_NAME:=Automatic
PKG_VERSION:=0.8.2
PKG_RELEASE:=1

PKG_SOURCE_URL:=https://github.com/1100101/Automatic/archive
PKG_SOURCE:=v$(PKG_VERSION).tar.gz
PKG_MD5SUM:=8245e9765d043180266755111499f794

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

define Package/automatic/conffiles
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

