#
# Copyright (C) 2009-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=nginx
PKG_VERSION:=1.2.8
PKG_RELEASE:=4

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://nginx.org/download/
PKG_MD5SUM:=b9f7640cca35196d0e719ac5da24c7bd

################################################################################################

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

################################################################################################


PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=libopenssl zlib

PKG_CONFIG_DEPENDS := \
	CONFIG_NGINX_STUB_STATUS \
	CONFIG_NGINX_FLV \
	CONFIG_NGINX_SSL \
	CONFIG_NGINX_DAV \
	CONFIG_NGINX_LUA \
	CONFIG_NGINX_RTMP \
	CONFIG_NGINX_PCRE \
	CONFIG_NGINX_HTTP_CACHE \
	CONFIG_NGINX_HTTP_CHARSET \
	CONFIG_NGINX_HTTP_GZIP \
	CONFIG_NGINX_HTTP_SSI \
	CONFIG_NGINX_HTTP_USERID \
	CONFIG_NGINX_HTTP_ACCESS \
	CONFIG_NGINX_HTTP_AUTH_BASIC \
	CONFIG_NGINX_HTTP_AUTOINDEX \
	CONFIG_NGINX_HTTP_GEO \
	CONFIG_NGINX_HTTP_MAP \
	CONFIG_NGINX_HTTP_SPLIT_CLIENTS \
	CONFIG_NGINX_HTTP_REFERER \
	CONFIG_NGINX_HTTP_REWRITE \
	CONFIG_NGINX_HTTP_PROXY \
	CONFIG_NGINX_HTTP_FASTCGI \
	CONFIG_NGINX_HTTP_UWSGI \
	CONFIG_NGINX_HTTP_SCGI \
	CONFIG_NGINX_HTTP_MEMCACHED \
	CONFIG_NGINX_HTTP_LIMIT_CONN \
	CONFIG_NGINX_HTTP_LIMIT_REQ \
	CONFIG_NGINX_HTTP_EMPTY_GIF \
	CONFIG_NGINX_HTTP_BROWSER \
	CONFIG_NGINX_HTTP_UPSTREAM_IP_HASH

include $(INCLUDE_DIR)/package.mk

################################################################################################

define Package/nginx/Default
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=Web Servers/Proxies
	TITLE:=Nginx web server ($(1))
	URL:=http://nginx.org/
	DEPENDS:=$(3)
	MENU:=$(2)
	VARIANT:=$(1)
endef

Package/nginx-basic=$(call Package/nginx/Default,basic,0,+libpcre)
Package/nginx-full=$(call Package/nginx/Default,full,0,+libpcre +liblua)
Package/nginx-custom=$(call Package/nginx/Default,custom,1,+NGINX_PCRE:libpcre +NGINX_LUA:liblua)

################################################################################################

define Package/nginx/description/Default
	nginx is an HTTP and reverse proxy server, as well as a mail proxy server,
	written by Igor Sysoev. $(1)
endef

Package/nginx-basic/description=$(call Package/nginx/description/Default,Built with basic set of modules)
Package/nginx-full/description=$(call Package/nginx/description/Default,Built with full set of modules)
Package/nginx-custom/description=$(call Package/nginx/description/Default,Built with custom set of modules)


################################################################################################

define Package/nginx-custom/config
    source "$(SOURCE)/Config.in"
endef

################################################################################################

ifeq ($(BUILD_VARIANT),basic)
CONFIG_NGINX_PCRE:=y

CONFIG_IPV6:=n

CONFIG_NGINX_STUB_STATUS:=n

CONFIG_NGINX_FLV:=n
CONFIG_NGINX_SSL:=y
CONFIG_NGINX_DAV:=n
CONFIG_NGINX_LUA:=n
CONFIG_NGINX_RTMP:=n

CONFIG_NGINX_HTTP_CACHE:=n
CONFIG_NGINX_HTTP_CHARSET:=y
CONFIG_NGINX_HTTP_GZIP:=y
CONFIG_NGINX_HTTP_SSI:=y
CONFIG_NGINX_HTTP_USERID:=y
CONFIG_NGINX_HTTP_ACCESS:=y
CONFIG_NGINX_HTTP_AUTH_BASIC:=y
CONFIG_NGINX_HTTP_AUTOINDEX:=y
CONFIG_NGINX_HTTP_GEO:=y
CONFIG_NGINX_HTTP_MAP:=y
CONFIG_NGINX_HTTP_SPLIT_CLIENTS:=y
CONFIG_NGINX_HTTP_REFERER:=y
CONFIG_NGINX_HTTP_REWRITE:=y
CONFIG_NGINX_HTTP_PROXY:=y
CONFIG_NGINX_HTTP_FASTCGI:=y
CONFIG_NGINX_HTTP_UWSGI:=y
CONFIG_NGINX_HTTP_SCGI:=y
CONFIG_NGINX_HTTP_MEMCACHED:=y
CONFIG_NGINX_HTTP_LIMIT_CONN:=y
CONFIG_NGINX_HTTP_LIMIT_REQ:=y
CONFIG_NGINX_HTTP_EMPTY_GIF:=y
CONFIG_NGINX_HTTP_BROWSER:=y
CONFIG_NGINX_HTTP_UPSTREAM_IP_HASH:=y
endif

ifeq ($(BUILD_VARIANT),full)
CONFIG_NGINX_PCRE:=y

CONFIG_IPV6:=n

CONFIG_NGINX_STUB_STATUS:=y

CONFIG_NGINX_FLV:=y
CONFIG_NGINX_SSL:=y
CONFIG_NGINX_DAV:=y
CONFIG_NGINX_LUA:=y
CONFIG_NGINX_RTMP:=y

CONFIG_NGINX_HTTP_CACHE:=n
CONFIG_NGINX_HTTP_CHARSET:=y
CONFIG_NGINX_HTTP_GZIP:=y
CONFIG_NGINX_HTTP_SSI:=y
CONFIG_NGINX_HTTP_USERID:=y
CONFIG_NGINX_HTTP_ACCESS:=y
CONFIG_NGINX_HTTP_AUTH_BASIC:=y
CONFIG_NGINX_HTTP_AUTOINDEX:=y
CONFIG_NGINX_HTTP_GEO:=y
CONFIG_NGINX_HTTP_MAP:=y
CONFIG_NGINX_HTTP_SPLIT_CLIENTS:=y
CONFIG_NGINX_HTTP_REFERER:=y
CONFIG_NGINX_HTTP_REWRITE:=y
CONFIG_NGINX_HTTP_PROXY:=y
CONFIG_NGINX_HTTP_FASTCGI:=y
CONFIG_NGINX_HTTP_UWSGI:=y
CONFIG_NGINX_HTTP_SCGI:=y
CONFIG_NGINX_HTTP_MEMCACHED:=y
CONFIG_NGINX_HTTP_LIMIT_CONN:=y
CONFIG_NGINX_HTTP_LIMIT_REQ:=y
CONFIG_NGINX_HTTP_EMPTY_GIF:=y
CONFIG_NGINX_HTTP_BROWSER:=y
CONFIG_NGINX_HTTP_UPSTREAM_IP_HASH:=y
endif


################################################################################################

config_files=nginx.conf mime.types fastcgi_params koi-utf koi-win win-utf

################################################################################################

define Package/nginx-$(BUILD_VARIANT)/conffiles
/etc/nginx/
endef

################################################################################################

ADDITIONAL_MODULES:=

ifeq ($(CONFIG_IPV6),y)
	ADDITIONAL_MODULES += --with-ipv6
endif
ifeq ($(CONFIG_NGINX_STUB_STATUS),y)
	ADDITIONAL_MODULES += --with-http_stub_status_module
endif
ifeq ($(CONFIG_NGINX_FLV),y)
	ADDITIONAL_MODULES += --with-http_flv_module
endif
ifeq ($(CONFIG_NGINX_SSL),y)
	ADDITIONAL_MODULES += --with-http_ssl_module
else
	ADDITIONAL_MODULES += --without-http-cache
endif
ifeq ($(CONFIG_NGINX_DAV),y)
	ADDITIONAL_MODULES += --with-http_dav_module
endif
ifeq ($(CONFIG_NGINX_LUA),y)
	ADDITIONAL_MODULES += --add-module=$(PKG_BUILD_DIR)/lua-nginx
endif
ifeq ($(CONFIG_NGINX_RTMP),y)
	ADDITIONAL_MODULES += --add-module=$(PKG_BUILD_DIR)/rtmp-nginx
endif
ifneq ($(CONFIG_NGINX_HTTP_CACHE),y)
	ADDITIONAL_MODULES += --without-http-cache
endif
ifneq ($(CONFIG_NGINX_PCRE),y)
	ADDITIONAL_MODULES += --without-pcre
endif
ifneq ($(CONFIG_NGINX_HTTP_CHARSET),y)
	ADDITIONAL_MODULES += --without-http_charset_module
endif
ifneq ($(CONFIG_NGINX_HTTP_GZIP),y)
	ADDITIONAL_MODULES += --without-http_gzip_module
endif
ifneq ($(CONFIG_NGINX_HTTP_SSI),y)
	ADDITIONAL_MODULES += --without-http_ssi_module
endif
ifneq ($(CONFIG_NGINX_HTTP_USERID),y)
	ADDITIONAL_MODULES += --without-http_userid_module
endif
ifneq ($(CONFIG_NGINX_HTTP_ACCESS),y)
	ADDITIONAL_MODULES += --without-http_access_module
endif
ifneq ($(CONFIG_NGINX_HTTP_AUTH_BASIC),y)
	ADDITIONAL_MODULES += --without-http_auth_basic_module
endif
ifneq ($(CONFIG_NGINX_HTTP_AUTOINDEX),y)
	ADDITIONAL_MODULES += --without-http_autoindex_module
endif
ifneq ($(CONFIG_NGINX_HTTP_GEO),y)
	ADDITIONAL_MODULES += --without-http_geo_module
endif
ifneq ($(CONFIG_NGINX_HTTP_MAP),y)
	ADDITIONAL_MODULES += --without-http_map_module
endif
ifneq ($(CONFIG_NGINX_HTTP_SPLIT_CLIENTS),y)
	ADDITIONAL_MODULES += --without-http_split_clients_module
endif
ifneq ($(CONFIG_NGINX_HTTP_REFERER),y)
	ADDITIONAL_MODULES += --without-http_referer_module
endif
ifneq ($(CONFIG_NGINX_HTTP_REWRITE),y)
	ADDITIONAL_MODULES += --without-http_rewrite_module
endif
ifneq ($(CONFIG_NGINX_HTTP_PROXY),y)
	ADDITIONAL_MODULES += --without-http_proxy_module
endif
ifneq ($(CONFIG_NGINX_HTTP_FASTCGI),y)
	ADDITIONAL_MODULES += --without-http_fastcgi_module
endif
ifneq ($(CONFIG_NGINX_HTTP_UWSGI),y)
	ADDITIONAL_MODULES += --without-http_uwsgi_module
endif
ifneq ($(CONFIG_NGINX_HTTP_SCGI),y)
	ADDITIONAL_MODULES += --without-http_scgi_module
endif
ifneq ($(CONFIG_NGINX_HTTP_MEMCACHED),y)
	ADDITIONAL_MODULES += --without-http_memcached_module
endif
ifneq ($(CONFIG_NGINX_HTTP_LIMIT_CONN),y)
	ADDITIONAL_MODULES += --without-http_limit_conn_module
endif
ifneq ($(CONFIG_NGINX_HTTP_LIMIT_REQ),y)
	ADDITIONAL_MODULES += --without-http_limit_req_module
endif
ifneq ($(CONFIG_NGINX_HTTP_EMPTY_GIF),y)
	ADDITIONAL_MODULES += --without-http_empty_gif_module
endif
ifneq ($(CONFIG_NGINX_HTTP_BROWSER),y)
	ADDITIONAL_MODULES += --without-http_browser_module
endif
ifneq ($(CONFIG_NGINX_HTTP_UPSTREAM_IP_HASH),y)
	ADDITIONAL_MODULES += --without-http_upstream_ip_hash_module
endif


################################################################################################

define Build/Configure
    # TODO: fix --crossbuild
	(cd $(PKG_BUILD_DIR) ;\
		$(if $(CONFIG_NGINX_LUA),LUA_INC=$(STAGING_DIR)/usr/include LUA_LIB=$(STAGING_DIR)/usr/lib) \
		./configure \
		--crossbuild=Linux::$(ARCH) \
		--prefix="$(PREFIX_ZYX)" \
		--conf-path="$(ROOT_ZYX)/etc/nginx/nginx.conf" \
		--error-log-path="$(ROOT_ZYX)/var/log/nginx/error.log" \
		--pid-path="$(ROOT_ZYX)/var/run/nginx.pid" \
		--lock-path="$(ROOT_ZYX)/var/lock/nginx.lock" \
		--http-log-path="$(ROOT_ZYX)/var/log/nginx/access.log" \
		--http-client-body-temp-path="$(ROOT_ZYX)/var/lib/nginx/body" \
		--http-proxy-temp-path="$(ROOT_ZYX)/var/lib/nginx/proxy" \
		--http-fastcgi-temp-path="$(ROOT_ZYX)/var/lib/nginx/fastcgi" \
		--with-cc="$(TARGET_CC)" \
		--with-cc-opt="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
		--with-ld-opt="$(TARGET_LDFLAGS)" \
		$(ADDITIONAL_MODULES) )
endef

define Package/nginx-$(BUILD_VARIANT)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/sbin/nginx $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/nginx
	$(INSTALL_DATA) $(addprefix $(PKG_INSTALL_DIR)$(ROOT_ZYX)/etc/nginx/,$(config_files)) $(1)/etc/nginx/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/var/log/nginx
	$(INSTALL_DIR) $(1)/var/lib/nginx
	$(INSTALL_DIR) $(1)/var/run/
	$(INSTALL_DIR) $(1)/var/lock/
	$(INSTALL_DIR) $(1)/var/www/
	$(INSTALL_BIN) ./files/K50nginx $(1)/etc/init.d/K50nginx
	$(INSTALL_DATA) ./files/nginx.conf $(1)/etc/nginx/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/html/* $(1)/var/www/
endef

define Build/Prepare
	$(call Build/Prepare/Default)
ifeq ($(CONFIG_NGINX_LUA),y)
	$(call Prepare/lua-nginx)
endif
ifeq ($(CONFIG_NGINX_RTMP),y)
	$(call Prepare/rtmp-nginx)
endif
endef

define Download/lua-nginx
	VERSION:=7c0948a08e8449f7a9ffbc35244954e6aaddac5b
	SUBDIR:=lua-nginx
	FILE:=lua-nginx-module-$(PKG_VERSION)-$$(VERSION).tar.gz
	URL:=git://github.com/chaoslawful/lua-nginx-module.git
	PROTO:=git
endef

define  Prepare/lua-nginx
	$(eval $(call Download,lua-nginx))
	gzip -dc $(DL_DIR)/$(FILE) | tar -C $(PKG_BUILD_DIR) $(TAR_OPTIONS)
	$(call PatchDir,$(PKG_BUILD_DIR),./patches-lua-nginx)
endef

define Download/rtmp-nginx
	VERSION:=7b5c5e99327c19600ab2bab89960ba217be52949
	SUBDIR:=rtmp-nginx
	FILE:=rtmp-nginx-module-$(PKG_VERSION)-$$(VERSION).tar.gz
	URL:=git://github.com/arut/nginx-rtmp-module.git
	PROTO:=git
endef

define  Prepare/rtmp-nginx
	$(eval $(call Download,rtmp-nginx))
	gzip -dc $(DL_DIR)/$(FILE) | tar -C $(PKG_BUILD_DIR) $(TAR_OPTIONS)
endef

$(eval $(call BuildPackage,nginx-basic))
$(eval $(call BuildPackage,nginx-full))
$(eval $(call BuildPackage,nginx-custom))
