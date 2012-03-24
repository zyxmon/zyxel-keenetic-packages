#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ffmpeg
PKG_VERSION:=0.10.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://ffmpeg.org/releases/
PKG_MD5SUM:=de1bd5fc4bbf3ef730a5361ee596fedd

FFMPEG_DECODERS:= \
	aac \
	ac3 \
	flac \
	h264 \
	jpegls \
	mp3 \
	mpeg1video \
	mpeg2video \
	mpeg4 \
	mpegvideo \
	png \
	wmav1 \
	wmav2 \
	zlib \

FFMPEG_PARSERS:= \
	aac \
	ac3 \
	h264 \
	mpeg4video \
	mpegaudio \
	mpegvideo \

FFMPEG_PROTOCOLS:= \
	file http pipe rtp tcp udp \

FFMPEG_CONFIGURE_DECODERS:= \
	$(foreach c, $(FFMPEG_DECODERS), \
		"--enable-decoder=$(c)" \
	)

FFMPEG_CONFIGURE_PARSERS:= \
	$(foreach c, $(FFMPEG_PARSERS), \
		"--enable-parser=$(c)" \
	)

FFMPEG_CONFIGURE_PROTOCOLS:= \
	$(foreach c, $(FFMPEG_PROTOCOLS), \
		"--enable-protocol=$(c)" \
	)

include $(INCLUDE_DIR)/package.mk

define Package/libffmpeg
$(call Package/ffmpeg/Default)
 SECTION:=libs
 CATEGORY:=Libraries
 TITLE+= libraries
 MENU:=1
endef

define Package/libffmeg/description
$(call Package/ffmpeg/Default/description)
 .
 This package contains FFmpeg shared libraries.
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
		\
		--enable-gpl \
		--enable-version3 \
		\
		--disable-doc \
		--disable-dxva2 \
		--disable-mmx \
		--disable-mmx2 \
		--enable-pthreads \
		--disable-optimizations \
		--enable-small \
		--disable-stripping \
		--enable-zlib \
		--enable-postproc \
		--enable-swscale \
		\
		--disable-bsfs \
		--disable-devices \
		--disable-filters \
		--disable-hwaccels \
		\
		--disable-bzlib \
		\
		--disable-encoders \
		--disable-decoders \
		$(FFMPEG_CONFIGURE_DECODERS) \
		--disable-muxers \
		--disable-parsers \
		$(FFMPEG_CONFIGURE_PARSERS) \
		\
		--disable-network \
		\
		--disable-protocols \
		--enable-protocol=file \
	)
endef

#		-------------------------------
#		--disable-protocols \
#		$(FFMPEG_CONFIGURE_PROTOCOLS) \
#		-------------------------------

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libav{codec,device,format,util} $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libav{codec,device,format,util}.{a,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libav{codec,device,format,util}.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libffmpeg/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libav{codec,device,format,util}.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libffmpeg))
