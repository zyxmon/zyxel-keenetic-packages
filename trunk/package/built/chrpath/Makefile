include $(TOPDIR)/rules.mk

PKG_NAME:=chrpath
PKG_VERSION:=0.14
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://alioth.debian.org/frs/download.php/file/3648/
#PKG_MD5SUM:=d44f79353950fb5b97bcdd6ffd0a186c

include $(INCLUDE_DIR)/package.mk

define Package/chrpath
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=RPATH changer/viewer
	URL:=https://alioth.debian.org/projects/chrpath/
endef

define Package/chrpath/description
	Command line tool to adjust the RPATH or RUNPATH of ELF binaries.
endef

define Package/chrpath/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/chrpath $(1)/usr/bin/
endef

$(eval $(call BuildPackage,chrpath))
