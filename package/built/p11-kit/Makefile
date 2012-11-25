include $(TOPDIR)/rules.mk

PKG_NAME:=p11-kit
PKG_VERSION:=0.13
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.gz
PKG_SOURCE_URL:=https://launchpad.net/ubuntu/+archive/primary/+files
PKG_MD5_SUM:=3892bc07ff54b1b391aad4628911801d

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/p11-kit
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=PKCS#11 is a standard for accessing crypto objects like keys and certificates and performing cryptographic operations on them. It's often used together with smart cards.
  URL:=http://p11-glue.freedesktop.org/
  DEPENDS:=$(ICONV_DEPENDS) $(INTL_DEPENDS)
endef

define Package/p11-kit/description
 PKCS#11 is a standard for accessing crypto objects like keys and certificates and performing cryptographic operations on them. It's often used together with smart cards.
endef

define Package/p11-kit/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libp11-kit.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/p11-kit-proxy.so $(1)/usr/lib
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/include/p11-kit-1 $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libp11-kit.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/p11-kit-1.pc $(1)/usr/lib/pkgconfig/
endef
$(eval $(call BuildPackage,p11-kit))
