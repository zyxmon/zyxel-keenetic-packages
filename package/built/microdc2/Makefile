include $(TOPDIR)/rules.mk


PKG_NAME:=microdc2
PKG_VERSION:=0.15.6
PKG_RELEASE:=3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://savio.km.ua/download/microdc2
PKG_MD5SUM:=
PKG_INSTALL:=1
#PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/microdc2
	SECTION:=net
	CATEGORY:=Network
	TITLE:=microdc2
	DEPENDS:=$(ICONV_DEPENDS) +libxml2 +libreadline +libncurses +iconv
	URL:=http://corsair626.no-ip.org/microdc
endef

define Package/microdc2/description
	A command-line based Direct Connect client.
endef

CONFIGURE_PREFIX:=$(PREFIX_ZYX)

CONFIGURE_ARGS += \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--with-libiconv-prefix="$(ICONV_PREFIX)" \
	--disable-static \
	--disable-rparh


define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LTLIBICONV="" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		install
endef

define Package/microdc2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,microdc2))
