#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=db51
PKG_VERSION:=5.1.25.NC
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/db-$(PKG_VERSION)
PKG_SOURCE:=db-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://download.oracle.com/berkeley-db/
PKG_MD5SUM:=39b7ccd4ff3d2ca4498b45a77fce3e80

PKG_FIXUP:=libtool
PKG_LIBTOOL_PATHS:=. build_unix
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libdb51
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libxml2
  TITLE:=Berkeley DB library (5.1)
  URL:=http://www.sleepycat.com/products/db.shtml
  MAINTAINER:=W. Michael Petullo <mike@flyn.org>
endef

define Package/libdb51/description
  Berkeley DB library (5.1).
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR)/build_unix; rm -f config.cache; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		../dist/configure \
			--target=$(GNU_TARGET_NAME) \
			--host=$(GNU_TARGET_NAME) \
			--build=$(GNU_HOST_NAME) \
			--program-prefix="" \
			--program-suffix="" \
			--prefix=/usr \
			--exec-prefix=/usr \
			--bindir=/usr/bin \
			--datadir=/usr/share \
			--includedir=/usr/include \
			--infodir=/usr/share/info \
			--libdir=/usr/lib \
			--libexecdir=/usr/lib \
			--localstatedir=/var \
			--mandir=/usr/share/man \
			--sbindir=/usr/sbin \
			--sysconfdir=/etc \
			$(DISABLE_NLS) \
			--enable-shared \
			--enable-static \
			--disable-java \
			--disable-cxx \
			--with-mutex=MIPS/gcc-assembly \
			--disable-tcl \
			--enable-compat185 \
			--enable-smallbuild \
			--disable-debug \
			--enable-cryptography \
	);
endef
#			--with-mutex=UNIX/fcntl \

TARGET_CFLAGS += $(FPIC)

define Build/Compile
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/build_unix \
		DESTDIR="$(PKG_INSTALL_DIR)" all
	$(MAKE) -C $(PKG_BUILD_DIR)/build_unix \
		DESTDIR="$(PKG_INSTALL_DIR)" install
endef

define Package/libdb51/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libdb-*.so $(1)/usr/lib/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/db.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libdb*.{a,so} $(1)/usr/lib
endef

$(eval $(call BuildPackage,libdb51))
