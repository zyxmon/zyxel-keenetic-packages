# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=v4l-info
PKG_VERSION:=3.102
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/v4l-info
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=v4l-info
endef

define Package/v4l-info/description
  Video 4 Linux info/ sourses are taken from xawtv version 3.102 http://linuxtv.org/downloads/xawtv/
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-s -o $(PKG_BUILD_DIR)/v4l-info \
				$(PKG_BUILD_DIR)/v4l-info.c \
				$(PKG_BUILD_DIR)/struct-dump.c \
				$(PKG_BUILD_DIR)/struct-v4l.c \
				$(PKG_BUILD_DIR)/struct-v4l2.c
endef

define Package/v4l-info/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/v4l-info $(1)/bin/v4l-info
endef

$(eval $(call BuildPackage,v4l-info))
