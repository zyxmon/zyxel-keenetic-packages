#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

SOUND_MENU:=Sound Support

define KernelPackage/pcspkr
  SUBMENU:=$(SOUND_MENU)
  TITLE:=PC speaker support
  KCONFIG:=CONFIG_INPUT_PCSPKR
  FILES:=$(LINUX_DIR)/drivers/input/misc/pcspkr.ko
endef

define KernelPackage/pcspkr/description
 This enables sounds (tones) through the pc speaker
endef

$(eval $(call KernelPackage,pcspkr))


# allow targets to override the soundcore stuff
SOUNDCORE_LOAD ?= \
	soundcore \
	snd \
	snd-page-alloc \
	snd-hwdep \
	snd-seq-device \
	snd-rawmidi \
	snd-timer \
	snd-pcm \
	snd-mixer-oss \
	snd-pcm-oss

SOUNDCORE_FILES ?= \
	$(LINUX_DIR)/sound/soundcore.ko \
	$(LINUX_DIR)/sound/core/snd.ko \
	$(LINUX_DIR)/sound/core/snd-page-alloc.ko \
	$(LINUX_DIR)/sound/core/snd-hwdep.ko \
	$(LINUX_DIR)/sound/core/seq/snd-seq-device.ko \
	$(LINUX_DIR)/sound/core/snd-rawmidi.ko \
	$(LINUX_DIR)/sound/core/snd-timer.ko \
	$(LINUX_DIR)/sound/core/snd-pcm.ko \
	$(LINUX_DIR)/sound/core/oss/snd-mixer-oss.ko \
	$(LINUX_DIR)/sound/core/oss/snd-pcm-oss.ko

define KernelPackage/sound-core
  SUBMENU:=$(SOUND_MENU)
  TITLE:=Sound support
  DEPENDS:=@AUDIO_SUPPORT
  KCONFIG:= \
	CONFIG_SOUND \
	CONFIG_SND \
	CONFIG_SND_HWDEP \
	CONFIG_SND_RAWMIDI \
	CONFIG_SND_TIMER \
	CONFIG_SND_PCM \
	CONFIG_SND_SEQUENCER \
	CONFIG_SND_VIRMIDI \
	CONFIG_SND_SEQ_DUMMY \
	CONFIG_SND_SEQUENCER_OSS=y \
	CONFIG_HOSTAUDIO \
	CONFIG_SND_PCM_OSS \
	CONFIG_SND_PCM_OSS_PLUGINS=y \
	CONFIG_SND_MIXER_OSS=m \
	CONFIG_SOUND_OSS_CORE_PRECLAIM=y \
	CONFIG_SND_DYNAMIC_MINORS=n \
	CONFIG_SND_SUPPORT_OLD_API=y \
	CONFIG_SND_VERBOSE_PROCFS=y \
	CONFIG_SND_VERBOSE_PRINTK=n \
	CONFIG_SND_DEBUG=n \
	CONFIG_SOUND_PRIME=n \
	CONFIG_SND_DUMMY=n \
	CONFIG_SND_MTPAV=n \
	CONFIG_SND_SERIAL_U16550=n \
	CONFIG_SND_MPU401=n \
	CONFIG_SND_AD1889=n \
	CONFIG_SND_ALS300=n \
	CONFIG_SND_ALI5451=n \
	CONFIG_SND_ATIIXP=n \
	CONFIG_SND_ATIIXP=n \
	CONFIG_SND_ATIIXP_MODEM=n \
	CONFIG_SND_AU8810=n \
	CONFIG_SND_AU8820=n \
	CONFIG_SND_AU8830=n \
	CONFIG_SND_AZT3328=n \
	CONFIG_SND_BT87X=n \
	CONFIG_SND_CA0106=n \
	CONFIG_SND_CMIPCI=n \
	CONFIG_SND_CS4281=n \
	CONFIG_SND_CS46XX=n \
	CONFIG_SND_DARLA20=n \
	CONFIG_SND_GINA20=n \
	CONFIG_SND_LAYLA20=n \
	CONFIG_SND_DARLA24=n \
	CONFIG_SND_GINA24=n \
	CONFIG_SND_LAYLA24=n \
	CONFIG_SND_MONA=n \
	CONFIG_SND_MIA=n \
	CONFIG_SND_ECHO3G=n \
	CONFIG_SND_INDIGO=n \
	CONFIG_SND_INDIGOIO=n \
	CONFIG_SND_INDIGODJ=n \
	CONFIG_SND_EMU10K1=n \
	CONFIG_SND_EMU10K1X=n \
	CONFIG_SND_ENS1370=n \
	CONFIG_SND_ENS1371=n \
	CONFIG_SND_ES1938=n \
	CONFIG_SND_ES1968=n \
	CONFIG_SND_FM801=n \
	CONFIG_SND_HDA_INTEL=n \
	CONFIG_SND_HDSP=n \
	CONFIG_SND_HDSPM=n \
	CONFIG_SND_ICE1712=n \
	CONFIG_SND_ICE1724=n \
	CONFIG_SND_INTEL8X0=n \
	CONFIG_SND_INTEL8X0M=n \
	CONFIG_SND_KORG1212=n \
	CONFIG_SND_MAESTRO3=n \
	CONFIG_SND_MIXART=n \
	CONFIG_SND_NM256=n \
	CONFIG_SND_PCXHR=n \
	CONFIG_SND_RIPTIDE=n \
	CONFIG_SND_RME32=n \
	CONFIG_SND_RME96=n \
	CONFIG_SND_RME9652=n \
	CONFIG_SND_SONICVIBES=n \
	CONFIG_SND_TRIDENT=n \
	CONFIG_SND_VIA82XX=n \
	CONFIG_SND_VIA82XX_MODEM=n \
	CONFIG_SND_VX222=n \
	CONFIG_SND_YMFPCI=n \
	CONFIG_SND_USB_CAIAQ=n
  FILES:=$(SOUNDCORE_FILES)
  $(call AddDepends/input)
endef

define KernelPackage/sound-core/uml
  FILES:= \
	$(LINUX_DIR)/sound/soundcore.ko \
	$(LINUX_DIR)/arch/um/drivers/hostaudio.ko
endef

define KernelPackage/sound-core/description
 Kernel modules for sound support
endef

$(eval $(call KernelPackage,sound-core))


define AddDepends/sound
  SUBMENU:=$(SOUND_MENU)
  DEPENDS+=kmod-sound-core $(1) @!TARGET_uml
endef


define KernelPackage/ac97
  TITLE:=ac97 controller
  KCONFIG:=CONFIG_SND_AC97_CODEC
  FILES:= \
	$(LINUX_DIR)/sound/ac97_bus.ko \
	$(LINUX_DIR)/sound/pci/ac97/snd-ac97-codec.ko 
  $(call AddDepends/sound)
endef

define KernelPackage/ac97/description
 The ac97 controller
endef

$(eval $(call KernelPackage,ac97))


define KernelPackage/sound-seq
  TITLE:=Sequencer support
  FILES:= \
	$(LINUX_DIR)/sound/core/seq/snd-seq.ko \
	$(LINUX_DIR)/sound/core/seq/snd-seq-midi-event.ko \
	$(LINUX_DIR)/sound/core/seq/snd-seq-midi.ko
  $(call AddDepends/sound)
endef

define KernelPackage/sound-seq/description
 Kernel modules for sequencer support
endef

$(eval $(call KernelPackage,sound-seq))


define KernelPackage/sound-i8x0
  TITLE:=Intel/SiS/nVidia/AMD/ALi AC97 Controller
  DEPENDS:=+kmod-ac97
  KCONFIG:=CONFIG_SND_INTEL8X0
  FILES:=$(LINUX_DIR)/sound/pci/snd-intel8x0.ko
  $(call AddDepends/sound)
endef

define KernelPackage/sound-i8x0/description
 support for the integrated AC97 sound device on motherboards
 with Intel/SiS/nVidia/AMD chipsets, or ALi chipsets using 
 the M5455 Audio Controller.
endef

$(eval $(call KernelPackage,sound-i8x0))


define KernelPackage/sound-cs5535audio
  TITLE:=CS5535 PCI Controller
  DEPENDS:=+kmod-ac97
  KCONFIG:=CONFIG_SND_CS5535AUDIO
  FILES:=$(LINUX_DIR)/sound/pci/cs5535audio/snd-cs5535audio.ko
  $(call AddDepends/sound)
endef

define KernelPackage/sound-cs5535audio/description
 support for the integrated AC97 sound device on olpc
endef

$(eval $(call KernelPackage,sound-cs5535audio))


define KernelPackage/sound-soc-core
  TITLE:=SoC sound support
  KCONFIG:= \
	CONFIG_SND_SOC \
	CONFIG_SND_SOC_ALL_CODECS=n
  FILES:=$(LINUX_DIR)/sound/soc/snd-soc-core.ko
  $(call AddDepends/sound)
endef

$(eval $(call KernelPackage,sound-soc-core))


define KernelPackage/sound-soc-ac97
  TITLE:=AC97 Codec support
  KCONFIG:=CONFIG_SND_SOC_AC97_CODEC
  FILES:=$(LINUX_DIR)/sound/soc/codecs/snd-soc-ac97.ko
  DEPENDS:=+kmod-ac97 +kmod-sound-soc-core
  $(call AddDepends/sound)
endef

$(eval $(call KernelPackage,sound-soc-ac97))
