# 
# Copyright (C) 2010-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
#
#
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sd-idle
PKG_VERSION:=2.6
PKG_RELEASE:=1

#PKG_BUILD_PARALLEL:=0

include $(INCLUDE_DIR)/package.mk

define Package/sd-idle
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Disk idle spindown program
  URL:=http://tomatousb.org/forum/t-271603/sd-idle-2-6-disk-idle-spindown-program-for-2-6
endef

define Package/sd-idle/description
  sd-idle-2.6 is a disk idle spindown program for router firmware based on linux 2.6.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

#TARGET_CFLAGS += $(FPIC)

define Package/sd-idle/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sd-idle-2.6 $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/sd-idle.init $(1)/etc/init.d/sd-idle
endef

$(eval $(call BuildPackage,sd-idle))
