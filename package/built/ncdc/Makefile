include $(TOPDIR)/rules.mk

PKG_NAME:=ncdc
PKG_VERSION:=1.10
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://dev.yorhel.nl/download

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/ncdc
	SUBMENU:=P2P
	SECTION:=net
	CATEGORY:=Network
	TITLE:=ncdc is a modern and lightweight DC client
	DEPENDS:=+libncursesw +bzip2 +libsqlite3 +glib2 +libxml2 +libgnutls $(ICONV_DEPENDS)
	URL:=http://dev.yorhel.nl/ncdc
endef

define Package/ncdc/description
	ncdc is a modern and lightweight direct connect client with a friendly
	ncurses interface.
endef

CONFIGURE_ARGS += \
	ac_cv_file__git_=no

TARGET_LDFLAGS += \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib

define Package/ncdc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/ncdc $(1)/usr/bin/
endef
$(eval $(call BuildPackage,ncdc))

