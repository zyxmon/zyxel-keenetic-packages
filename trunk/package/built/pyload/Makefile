include $(TOPDIR)/rules.mk

PKG_NAME:=pyload
PKG_VERSION:=1.0.0
PKG_RELEASE:=2

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/pyload
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:= +js +python-django +py-openssl +python-pil +python-sqlite3 +python-crypto +python-curl +tesseract +tesseract-eng-data +screen
	TITLE:=pyLoad metapackage and scrips
	PKGARCH:=all
endef

define Package/pyload/description
pyLoad is a fast, lightweight and full featured download manager written in python for many One-Click-Hoster, container formats like DLC, video sites or just plain http/ftp links. It aims for low hardware requirements and platform independence to be runnable on all kind of systems (desktop pc, netbook, NAS, router). 
Despite its strict restriction it is packed full of features just like webinterface, captcha recognition, unrar and much more. 	
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef


define Package/pyload/install
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/pyLoadSetup.sh $(1)/usr/bin/pyLoadSetup.sh
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K88pyload $(1)/etc/init.d/K88pyload
endef

$(eval $(call BuildPackage,pyload))
