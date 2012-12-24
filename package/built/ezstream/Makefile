#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ezstream
PKG_VERSION:=0.5.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://downloads.xiph.org/releases/ezstream/
PKG_MD5SUM:=1be68119d44fbe71454a901fa650a359

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

define Package/ezstream
  SECTION:=multimedia
  CATEGORY:=Multimedia
  DEPENDS:=+libshout +taglib +libiconv-full +libxml2
  TITLE:=Ezstream is a command line source client for Icecast media streaming servers
  URL:=http://www.icecast.org/
endef

define Package/ezstream/description
	Ezstream is a command line source client for Icecast media 
	streaming servers. It began as the successor of the old "shout" 
	utility, and has since gained a lot of useful features.
	In its basic mode of operation, it streams media files or data 
	from standard input without reencoding and thus requires only 
	very little CPU resources. It can also use various external 
	decoders and encoders to reencode from one format to another, 
	and stream the result to an Icecast server. Additional features 
	include scriptable playlist and metadata handling. All of its 
	features make ezstream a very flexible source client.
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

define Package/ezstream/install
	$(INSTALL_DIR) $(1)/etc/ezstream
	$(INSTALL_CONF) ./files/* $(1)/etc/ezstream/
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/usr/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ezstream))
