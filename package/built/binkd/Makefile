# 
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=binkd
PKG_VERSION:=1.0a-561
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://keenetic.zyxmon.org/sources/
#PKG_SOURCE_URL:=:pserver:binkd@cvs.happy.kiev.ua:/cvs
#PKG_SOURCE_SUBDIR:=binkd
#PKG_SOURCE_VERSION:=-D'08/19/2011'
#PKG_SOURCE_PROTO:=cvs

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/binkd
    TITLE:=Fidonet mailer
    SECTION:=utilities
    CATEGORY:=Utilities
    URL:=http://binkd.grumbler.org
    DEPENDS:=+bzip2
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) $(PKG_BUILD_DIR)/mkfls/unix/* $(PKG_BUILD_DIR)
endef

define Package/binkd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/binkd $(1)/usr/bin/
endef

$(eval $(call BuildPackage,binkd))
