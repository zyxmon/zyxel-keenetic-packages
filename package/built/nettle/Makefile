include $(TOPDIR)/rules.mk

PKG_NAME:=nettle
PKG_VERSION:=2.5
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@GNU/nettle

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/nettle
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=The Nettle package contains the low-level cryptographic library that is designed to fit easily in many contexts.
  URL:=http://www.linuxfromscratch.org/blfs/view/svn/postlfs/nettle.html
endef

define Package/nettle-tools
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=nettle
  TITLE:=Tools for nettle
endef

define Package/nettle/description
 The Nettle package contains the low-level cryptographic library that is designed to fit easily in many contexts.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/include/nettle $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib{nettle,hogweed}.{a,so*} $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/{hogweed,nettle}.pc $(1)/usr/lib/pkgconfig/
endef

define Package/nettle/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnettle.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libhogweed.so* $(1)/usr/lib
endef

define Package/nettle-tools/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/nettle-lfib-stream $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/pkcs1-conv $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/sexp-conv $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/nettle-hash $(1)/usr/sbin
endef

$(eval $(call BuildPackage,nettle))
$(eval $(call BuildPackage,nettle-tools))
