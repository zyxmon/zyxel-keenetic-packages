#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pyOpenSSL
PKG_VERSION:=0.12
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://pypi.python.org/packages/source/p/pyOpenSSL/
PKG_MD5SUM:=

PKG_BUILD_DEPENDS:=libopenssl

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/py-openssl
	SECTION:=lang-python
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=py-OpenSSL
	URL:=http://pypi.python.org
	DEPENDS:=+python
endef

define Package/py-openssl/description
Python wrapper around a small subset of the OpenSSL library.
endef

define Build/Compile
	$(call Build/Compile/PyMod,,install --prefix=/usr --root=$(PKG_INSTALL_DIR))
endef

define Package/py-openssl/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)/
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)/
endef

$(eval $(call BuildPackage,py-openssl))
