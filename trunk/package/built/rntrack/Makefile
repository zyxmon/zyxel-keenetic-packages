# 
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=rntrack
PKG_VERSION:=1_32
PKG_RELEASE:=1
	
PKG_SOURCE:=rntrack-$(PKG_VERSION)-src.tar.bz2
PKG_SOURCE_URL:=@SF/ftrack-as/RNtrack/1.32
PKG_MD5SUM:=b34eff2443efe378d14d8c77c531d2e1

include $(INCLUDE_DIR)/package.mk

PKG_UNPACK:=bzip2 -c -d $(DL_DIR)/$(PKG_SOURCE) | $(TAR) -C $(PKG_BUILD_DIR) --strip-components=1 -xf -

define Package/rntrack
    TITLE:=Fidonet Mail Tracker
    SECTION:=utilities
	DEPENDS:=+libstdcpp
    CATEGORY:=Utilities
    URL:=http://sourceforge.net/projects/ftrack-as
endef

define Package/rntrack/description
    RNTrack - A FTN Messages Tracker.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/MakeFiles/linux \
		CONFIG="$(ROOT_ZYX)/etc/rntrack/rntrack.cfg" \
		ENABLE_SCRIPTS=0 \
		ENABLE_LOG_PID=1 \
		ENABLE_SYSLOG_LOG_FORMAT=1 \
		CC="$(TARGET_CC)" \
		LDFLAGS="$(TARGET_LDFLAGS) -lstdc++" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		LIB="$(TARGET_CROSS)ar" \
		RANLIB="$(TARGET_CROSS)ranlib"
endef

define Package/rntrack/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/MakeFiles/linux/rntrack $(1)/usr/bin/
endef

$(eval $(call BuildPackage,rntrack))
