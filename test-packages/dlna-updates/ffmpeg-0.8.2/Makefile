#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ffmpeg
PKG_VERSION:=0.8.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://ffmpeg.org/releases/
PKG_MD5SUM:=967d481c98161582d149aced6e3b2f31

#libavcodec.so.52 McMCC
FFMPEG_DECODERS:= \
	h264 \
	jpegls \
	mpeg1video \
	mpeg2video \
	mpeg4 \
	mpegvideo \
	png \
	zlib \
	ac3 \
	atrac3 \
	mp3 \
	wmav1 \
	wmav2 \

FFMPEG_PARSERS:= \
	aac \
	ac3 \
	h264 \
	mpeg4video \
	mpegaudio \
	mpegvideo \

FFMPEG_PROTOCOLS:= \
	file http pipe rtp tcp udp

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
 DEPENDS+= +libpthread +zlib
 MENU:=1
endef

#define Package/libffmpeg/config
#source "$(SOURCE)/Config.in"
#endef

define Package/libffmeg/description
$(call Package/ffmpeg/Default/description)
 .
 This package contains FFmpeg shared libraries.
endef

ifneq ($(CONFIG_TARGET_x86),)
  TARGET_CFLAGS += -fomit-frame-pointer
endif

# XXX: add --disable-mmx & --disable-mmx2 to fix build failure on x86
# libpostproc/postprocess_template.c:2195: error: can't find a register in class 'GENERAL_REGS' while reloading 'asm'
# libpostproc/postprocess_template.c:3124: error: can't find a register in class 'GENERAL_REGS' while reloading 'asm'
# libpostproc/postprocess_template.c:3207: error: can't find a register in class 'GENERAL_REGS' while reloading 'asm'

define Build/Configure
	# this is *NOT* GNU configure
	( cd $(PKG_BUILD_DIR); \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		./configure \
		--enable-cross-compile \
		--cross-prefix="$(TARGET_CROSS)" \
		--arch="$(ARCH)" \
		--target-os=linux \
		--prefix="/usr" \
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
		--disable-muxers \
		--disable-parsers \
		--disable-protocols \
		$(FFMPEG_CONFIGURE_PROTOCOLS) \
	)
endef

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
