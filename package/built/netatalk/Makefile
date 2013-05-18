#
# Copyright (C) 2009-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=netatalk
PKG_VERSION:=2.2.1
PKG_RELEASE:=2a

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/netatalk
PKG_MD5SUM:=4a371025daf96adb73fa491efdbfef28

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

#PKG_BUILD_DEPENDS:=libopenssl

include $(INCLUDE_DIR)/package.mk
define Package/netatalk
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Filesystem
  DEPENDS:=+libopenssl +libdb51 +libgcrypt
  TITLE:=netatalk
  URL:=http://netatalk.sourceforge.net
  MAINTAINER:=W. Michael Petullo <mike@flyn.org>
endef

define Package/netatalk/decription
  Netatalk is a freely-available Open Source AFP fileserver.
  It also provides a kernel level implementation of the AppleTalk
  Protocol Suite.
endef

define Package/netatalk/conffiles
/etc/netatalk/afpd.conf
endef

EXTRA_CFLAGS += -I$(STAGING_DIR)/usr/include
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include

CONFIGURE_VARS += \
	CFLAGS="$(TARGET_CFLAGS) -std=gnu99 -DNO_DEBUG "

CONFIGURE_ARGS += \
	--disable-afs \
	--enable-hfs \
	--disable-debugging \
	--disable-shell-check \
	--disable-timelord \
	--disable-a2boot \
	--disable-cups \
	--disable-tcp-wrappers \
	--with-cnid-cdb-backend \
	--with-cnid-default-backend=cdb \
	--with-bdb="$(STAGING_DIR)/usr/" \
	--with-libgcrypt-dir="$(STAGING_DIR)/usr" \
	--with-ssl-dir="$(STAGING_DIR)/usr" \
	--with-uams-path="/media/DISK_A1/system/usr/lib/uams" \
	--disable-admin-group \
	--disable-srvloc \
	--disable-zeroconf \
	--without-shadow \
	--without-ldap \
	--with-pkgconfdir=/media/DISK_A1/system/etc/netatalk
	
define Package/netatalk/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib/uams
	$(INSTALL_DIR) $(1)/etc/netatalk
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/var/log
	$(INSTALL_DIR) $(1)/var/run
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/afppasswd $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/afpd $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/cnid_dbd $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/cnid_metad $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/media/DISK_A1/system/usr/lib/uams/*.so $(1)/usr/lib/uams/
	$(CP) ./files/AppleVolumes.default $(1)/etc/netatalk/
	$(CP) $(PKG_INSTALL_DIR)/media/DISK_A1/system/etc/netatalk/AppleVolumes.system $(1)/etc/netatalk/
	$(INSTALL_CONF) ./files/afpd.conf $(1)/etc/netatalk/
	$(INSTALL_BIN) ./files/S10afpd $(1)/etc/init.d/S10afpd
endef

define Package/netatalk/postinst
#!/bin/sh

echo "Creating timemachine dir..."
mkdir /media/DISK_A1/timemachine/
chmod 0777 /media/DISK_A1/timemachine/
echo "Starting netatalk..."
/media/DISK_A1/system/etc/init.d/S10afpd start

endef

$(eval $(call BuildPackage,netatalk))
