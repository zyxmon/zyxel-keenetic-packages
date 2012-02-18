# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=mini_inetd
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/mini_inetd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=mini inetd
endef

define Package/mini_inetd/description
  mini-inetd: Waits on a TCP socket, spawns a subprocess for each
  incoming connection to handle the connection.  Can be used to test
  servers that expect to be launched from inetd.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-s -o $(PKG_BUILD_DIR)/mini_inetd \
				$(PKG_BUILD_DIR)/mini_inetd.c
endef

define Package/mini_inetd/install	
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mini_inetd $(1)/bin/mini_inetd
endef

$(eval $(call BuildPackage,mini_inetd))
