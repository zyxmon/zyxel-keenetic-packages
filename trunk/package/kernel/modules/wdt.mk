# $Id$

WDT_MENU:=Watchdog Timer

define KernelPackage/rt_timer
  SUBMENU:=$(WDT_MENU)
  TITLE:=Ralink Watchdog and Soft Timer
  KCONFIG:=CONFIG_RALINK_TIMER
  FILES:=$(LINUX_DIR)/arch/mips/rt2880/rt_timer.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,10,rt_timer)
endef

define KernelPackage/rt_timer/description
  Ralink Timer Module.
endef

$(eval $(call KernelPackage,rt_timer))

define KernelPackage/ralink_wdt
  SUBMENU:=$(WDT_MENU)
  TITLE:=New Ralink Watchdog and Soft Timer
  KCONFIG:=CONFIG_RALINK_TIMER
  FILES:=$(LINUX_DIR)/arch/mips/rt2880/ralink_wdt.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,10,rt_timer)
endef

define KernelPackage/ralink_wdt/description
  New Ralink Timer Module.
endef

$(eval $(call KernelPackage,ralink_wdt))
