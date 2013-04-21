#
# Copyright (C) 2006-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ffmpeg
PKG_VERSION:=0.11.2
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://ffmpeg.org/releases/
PKG_MD5SUM:=93c1908022567b321df74db7214da6ac

PKG_LICENSE:=LGPLv2.1+ GPLv2+ LGPLv3
PKG_LICENSE_FILES:=COPYING.GPLv2 COPYING.GPLv3 COPYING.LGPLv2.1 COPYING.LGPLv3



include $(INCLUDE_DIR)/package.mk

define Package/ffmpeg/Default
	TITLE:=FFmpeg
	URL:=http://ffmpeg.mplayerhq.hu/
endef

define Package/ffmpeg/Default/description
 FFmpeg is a a software package that can record, convert and stream digital 
 audio and video in numerous formats.
endef


define Package/ffmpeg
$(call Package/ffmpeg/Default)
	SECTION:=multimedia
	CATEGORY:=Multimedia
	TITLE+= program
	DEPENDS+= +libffmpeg
endef

define Package/ffmpeg/description
$(call Package/ffmpeg/Default/description)
 .
 This package contains the FFmpeg command line tool.
endef


define Package/ffprobe
$(call Package/ffmpeg/Default)
	SECTION:=multimedia
	CATEGORY:=Multimedia
	TITLE+= CLI media identifier
	DEPENDS+= +libffmpeg
endef

define Package/ffprobe/description
$(call Package/ffmpeg/Default/description)
 .
 This package contains the FFprobe command line tool.
endef


define Package/ffserver
$(call Package/ffserver/Default)
	SECTION:=multimedia
	CATEGORY:=Multimedia
	TITLE+= streaming server
	DEPENDS+= +libffmpeg
endef

define Package/ffserver/description
$(call Package/ffmpeg/Default/description)
 .
 This package contains the FFmpeg streaming server.
endef


define Package/libffmpeg
$(call Package/ffmpeg/Default)
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE+= libraries
	DEPENDS+= +libbz2
endef



ifneq ($(CONFIG_TARGET_x86),)
	TARGET_CFLAGS += -fomit-frame-pointer
endif

define Build/Configure
	( cd $(PKG_BUILD_DIR); \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		./configure \
		--enable-cross-compile \
		--cross-prefix="$(TARGET_CROSS)" \
		--arch="$(ARCH)" \
		--target-os=linux \
		--prefix=/usr \
		--enable-shared \
		--enable-static \
		--disable-debug \
		--pkg-config="pkg-config" \
		\
		--enable-gpl \
		--enable-version3 \
		\
		--disable-asm \
		--disable-doc \
		--disable-dxva2 \
		--enable-pthreads \
		--disable-optimizations \
		--enable-small \
		--disable-stripping \
		--enable-zlib \
		--disable-outdevs \
	)
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/lib{avcodec,avdevice,avfilter,avformat,avutil,postproc,swresample,swscale} $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib{avcodec,avdevice,avfilter,avformat,avutil,postproc,swresample,swscale}.{a,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/lib{avcodec,avdevice,avfilter,avformat,avutil,postproc,swresample,swscale}.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libffmpeg/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib{avcodec,avdevice,avfilter,avformat,avutil,postproc,swresample,swscale}.so.* $(1)/usr/lib/
endef

define Package/ffmpeg/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/ffmpeg $(1)/usr/bin/
endef

define Package/ffprobe/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/ffprobe $(1)/usr/bin/
endef

define Package/ffserver/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/ffserver $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ffmpeg))
$(eval $(call BuildPackage,ffprobe))
$(eval $(call BuildPackage,ffserver))
$(eval $(call BuildPackage,libffmpeg))
