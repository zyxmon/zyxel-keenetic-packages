#
# YAMDI
# See ofsite for more information
#

include $(TOPDIR)/rules.mk

PKG_NAME:=yamdi
PKG_VERSION:=1.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/yamdi
PKG_MD5SUM:=14254b2a68f88feb3fead962bca5dc03

include $(INCLUDE_DIR)/package.mk

define Package/yamdi
	SECTION:=multimedia
	CATEGORY:=Multimedia
	TITLE:=Yet Another MetaData Injector for FLV
	URL:=http://yamdi.sourceforge.net/
endef

define Package/yamdi/description
	yamdi stands for Yet Another MetaData Injector
	and is a metadata injector for FLV files.
	It adds the onMetaData event to your FLV files.
	yamdi should run under *BSD, Linux and Windows
	and is published under the BSD license.
endef

define Package/yamdi/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/yamdi $(1)/usr/bin/
endef

$(eval $(call BuildPackage,yamdi))
