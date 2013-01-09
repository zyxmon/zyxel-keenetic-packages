# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=balance
PKG_VERSION:=3.54
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/balance
  SECTION:=net
  CATEGORY:=Network
  TITLE:=balance
endef

define Package/balance/description
  Balance
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-s -o $(PKG_BUILD_DIR)/balance \
				$(PKG_BUILD_DIR)/butils.c \
				$(PKG_BUILD_DIR)/balance.c
endef

define Package/balance/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/var/run/balance
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/balance $(1)/bin/balance
endef

$(eval $(call BuildPackage,balance))
