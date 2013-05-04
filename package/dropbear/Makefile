#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dropbear
PKG_VERSION:=2012.55
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:= \
	http://matt.ucc.asn.au/dropbear/releases/ \
	http://www.mirrors.wiretapped.net/security/cryptography/apps/ssh/dropbear/
PKG_MD5SUM:=8c784baec3054cdb1bb4bfa792c87812

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE libtomcrypt/LICENSE libtommath/LICENSE

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_PARALLEL:=1

ifeq ($(BUILD_VARIANT),storage)
PATCH_DIR:=spatches
endif

include $(INCLUDE_DIR)/package.mk

define Package/dropbear/Default
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Small SSH2 client/server
  URL:=http://matt.ucc.asn.au/dropbear/
endef

define Package/dropbear
  $(call Package/dropbear/Default)
  DEPENDS:=@TARGET_keenetic||TARGET_keeneticII
  VARIANT:=usb
endef

define Package/dropbear/Default/description
 A small SSH2 server/client designed for small memory environments.
endef

define Package/dropbear/description
  $(call Package/dropbear/Default/description)
  This package is built for usb.
endef

define Package/dropbear-storage
  $(call Package/dropbear/Default)
  TITLE+= (for internal storage)
  DEPENDS:=@TARGET_keenetic_storage||TARGET_keeneticII_storage
  VARIANT:=storage
endef

define Package/dropbear-storage/description
  $(call Package/dropbear/Default/description)
  This package is built for internal storage
endef


define Package/dropbearconvert
  $(call Package/dropbear/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Utility for converting SSH keys
endef

CONFIGURE_ARGS += \
	--with-shared \
	--disable-pam \
	--enable-openpty \
	--enable-syslog \
	$(if $(CONFIG_SHADOW_PASSWORDS),,--disable-shadow) \
	--disable-lastlog \
	--disable-utmp \
	--disable-utmpx \
	--disable-wtmp \
	--disable-wtmpx \
	--disable-loginfunc \
	--disable-pututline \
	--disable-pututxline \
	--disable-zlib \
	--enable-bundled-libtom

TARGET_CFLAGS += -DARGTYPE=3 -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

ifeq ($(BUILD_VARIANT),storage)
		BUILDPROGRAMS:=PROGRAMS="dropbear" MULTI=0 SCPPROGRESS=1
else
		BUILDPROGRAMS:=PROGRAMS="dropbear dbclient dropbearkey scp" MULTI=1 SCPPROGRESS=1
endif

define Build/Compile
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		$(BUILDPROGRAMS)
	+$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		PROGRAMS="dropbearconvert"
endef

define Package/dropbear/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dropbearmulti $(1)/usr/sbin/dropbear
	$(INSTALL_DIR) $(1)/usr/bin
	ln -sf ../sbin/dropbear $(1)/usr/bin/scp
	ln -sf ../sbin/dropbear $(1)/usr/bin/ssh
	ln -sf ../sbin/dropbear $(1)/usr/bin/dbclient
	ln -sf ../sbin/dropbear $(1)/usr/bin/dropbearkey
endef

define Package/dropbear-storage/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dropbear $(1)/usr/sbin/dropbear
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/S10dropbear $(1)/etc/init.d
endef

define Package/dropbearconvert/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dropbearconvert $(1)/usr/bin/dropbearconvert
endef

$(eval $(call BuildPackage,dropbear))
$(eval $(call BuildPackage,dropbear-storage))
$(eval $(call BuildPackage,dropbearconvert))
