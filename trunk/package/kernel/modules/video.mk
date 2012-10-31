#
# Copyright (C) 2009 David Cooper <dave@kupesoft.com>
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

VIDEO_MENU:=Video Support


define KernelPackage/fb
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=Framebuffer support
  DEPENDS:=@DISPLAY_SUPPORT
  KCONFIG:=CONFIG_FB
  FILES:=$(LINUX_DIR)/drivers/video/fb.ko
endef

define KernelPackage/fb/description
  Kernel support for framebuffers
endef

$(eval $(call KernelPackage,fb))

define KernelPackage/fb-cfb-fillrect
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=Framebuffer software rectangle filling support
  DEPENDS:=+kmod-fb
  KCONFIG:=CONFIG_FB_CFB_FILLRECT
  FILES:=$(LINUX_DIR)/drivers/video/cfbfillrect.ko
endef

define KernelPackage/fb-cfb-fillrect/description
  Kernel support for software rectangle filling
endef

$(eval $(call KernelPackage,fb-cfb-fillrect))


define KernelPackage/fb-cfb-copyarea
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=Framebuffer software copy area support
  DEPENDS:=+kmod-fb
  KCONFIG:=CONFIG_FB_CFB_COPYAREA
  FILES:=$(LINUX_DIR)/drivers/video/cfbcopyarea.ko
endef

define KernelPackage/fb-cfb-copyarea/description
  Kernel support for software copy area
endef

$(eval $(call KernelPackage,fb-cfb-copyarea))

define KernelPackage/fb-cfb-imgblt
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=Framebuffer software image blit support
  DEPENDS:=+kmod-fb
  KCONFIG:=CONFIG_FB_CFB_IMAGEBLIT
  FILES:=$(LINUX_DIR)/drivers/video/cfbimgblt.ko
endef

define KernelPackage/fb-cfb-imgblt/description
  Kernel support for software image blitting
endef

$(eval $(call KernelPackage,fb-cfb-imgblt))


define KernelPackage/video-core
  SUBMENU:=$(VIDEO_MENU)
  TITLE=Video4Linux support
  DEPENDS:=@PCI_SUPPORT||USB_SUPPORT
  KCONFIG:= \
	CONFIG_MEDIA_SUPPORT=m \
	CONFIG_VIDEO_DEV \
	CONFIG_VIDEO_V4L1=y \
	CONFIG_VIDEO_ALLOW_V4L1=y \
	CONFIG_VIDEO_CAPTURE_DRIVERS=y \
	CONFIG_V4L_USB_DRIVERS=y \
	CONFIG_V4L_PCI_DRIVERS=y \
	CONFIG_V4L_PLATFORM_DRIVERS=y \
	CONFIG_V4L_ISA_PARPORT_DRIVERS=y
  FILES:= \
	$(LINUX_DIR)/drivers/media/video/v4l2-common.ko \
	$(LINUX_DIR)/drivers/media/video/videodev.ko \
	$(LINUX_DIR)/drivers/media/video/v4l1-compat.ko \
	$(LINUX_DIR)/drivers/media/video/compat_ioctl32.ko
endef

define KernelPackage/video-core/description
 Kernel modules for Video4Linux support
endef

$(eval $(call KernelPackage,video-core))


define AddDepends/video
  SUBMENU:=$(VIDEO_MENU)
  DEPENDS+=kmod-video-core $(1)
endef


define KernelPackage/video-cpia2
  TITLE:=CPIA2 video driver
  DEPENDS:=@USB_SUPPORT
  KCONFIG:=CONFIG_VIDEO_CPIA2
  FILES:=$(LINUX_DIR)/drivers/media/video/cpia2/cpia2.ko
endef

define KernelPackage/video-cpia2/description
 Kernel modules for supporting CPIA2 USB based cameras.
endef

$(eval $(call KernelPackage,video-cpia2))


define KernelPackage/video-sn9c102
  TITLE:=SN9C102 Camera Chip support
  DEPENDS:=@USB_SUPPORT
  KCONFIG:=CONFIG_USB_SN9C102
  FILES:=$(LINUX_DIR)/drivers/media/video/sn9c102/sn9c102.ko
endef


define KernelPackage/video-sn9c102/description
 Kernel modules for supporting SN9C102
 camera chips.
endef

$(eval $(call KernelPackage,video-sn9c102))


define KernelPackage/video-pwc
  TITLE:=Philips USB webcam support
  DEPENDS:=@USB_SUPPORT
  KCONFIG:= \
	CONFIG_USB_PWC \
	CONFIG_USB_PWC_DEBUG=n
  FILES:= \
	$(LINUX_DIR)/drivers/media/video/pwc/pwc.ko
endef


define KernelPackage/video-pwc/description
 Kernel modules for supporting Philips USB based cameras.
endef

$(eval $(call KernelPackage,video-pwc))

define KernelPackage/video-uvc
  TITLE:=USB Video Class (UVC) support
  DEPENDS:=@USB_SUPPORT
  KCONFIG:= CONFIG_USB_VIDEO_CLASS \
	CONFIG_USB_VIDEO_CLASS_DEBUG=n
  FILES:=$(LINUX_DIR)/drivers/media/video/uvc/uvcvideo.ko
endef


define KernelPackage/video-uvc/description
 Kernel modules for supporting USB Video Class (UVC) devices.
endef

$(eval $(call KernelPackage,video-uvc))
