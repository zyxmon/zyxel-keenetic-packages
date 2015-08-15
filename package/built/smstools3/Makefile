include $(TOPDIR)/rules.mk

PKG_NAME:=smstools3
PKG_VERSION:=3.1.15
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://smstools3.kekekasvi.com/packages/
PKG_MD5SUM:=0241ef60e646fac1a06254a848e61ed7

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DEPENDS:=libiconv-full iconv

include $(INCLUDE_DIR)/package.mk

define Package/smstools3
	SECTION:=net
	CATEGORY:=Network
	TITLE:=smstools3
	DEPENDS:=+iconv
	URL:=http://smstools3.kekekasvi.com/
endef

define Package/smstools3/description
	The SMS Server Tools 3 is a SMS Gateway software which can send and receive
	short messages through GSM modems and mobile phones.
endef

TARGET_CFLAGS = -D NUMBER_OF_MODEMS=1
TARGET_CFLAGS += -D USE_ICONV
TARGET_CFLAGS += -D DISABLE_INET_SOCKET
TARGET_CFLAGS += -W -Wall
TARGET_CFLAGS += -D_FILE_OFFSET_BITS=64

TARGET_LDFLAGS += -liconv

BINDIR=/opt/bin

define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)/src" \
		CC="$(TARGET_CC)" \
		USER_LDFLAGS='$(TARGET_LFLAGS) $(EXTRA_LDFLAGS) -L"$(STAGING_DIR)/opt/lib/libiconv-full/lib"' \
		all
endef

define Package/smstools3/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/smsd $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scripts/{sendsms,sms2html,sms2unicode,unicode2sms} $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/examples/smsd.conf.easy $(1)/etc/smsd.conf
	$(INSTALL_DIR) $(1)/var/spool/sms/{incoming,outgoing,checked} $(1)/var/run
endef

$(eval $(call BuildPackage,smstools3))
