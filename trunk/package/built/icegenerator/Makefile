#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=icegenerator
PKG_VERSION:=0.5.5-pre2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/icegenerator
PKG_MD5SUM:=71515d79bc43ca53baffc96c4ada946a

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

define Package/icegenerator
  SECTION:=multimedia
  CATEGORY:=Multimedia
  DEPENDS:=+libshout +id3lib +libstdcpp
  TITLE:=IceGenerator is a direct stream generator for Icecast/Shoutcast
  URL:=http://www.becrux.com/
endef

define Package/icegenerator/description
	IceGenerator is a direct stream generator for Icecast/Shoutcast 
	servers. Streams MP3 to an Icecast/Shoutcast server without 
	resampling it, saving CPU. Features directories search, ID3, 
	M3U/PLS, META dynamic info and telnet interface.
endef

define Build/Configure
	$(call Build/Configure/Default, \
		--bindir="$(PREFIX_ZYX)/bin" \
		--sysconfdir="$(ROOT_ZYX)/etc" \
	)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Package/icegenerator/install
	$(INSTALL_DIR) $(1)/etc/icegenerator
	$(INSTALL_CONF) ./files/icegenerator.conf.dist $(1)/etc/icegenerator/icegenerator.conf
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,icegenerator))
