include $(TOPDIR)/rules.mk

PKG_NAME:=daemonize
PKG_VERSION:=1.7.3
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
include $(INCLUDE_DIR)/package.mk

define Package/daemonize
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=A tool to run a command as a daemon
	URL:=http://software.clapper.org/daemonize/
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/daemonize/description
	A tool to run a command as a daemon.
endef


define Package/daemonize/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/daemonize $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,daemonize))

