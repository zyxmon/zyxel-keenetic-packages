#
# Copyright (C) 2009-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=nginx
PKG_VERSION:=1.2.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://nginx.org/download/
PKG_MD5SUM:=4f5a55187a3d45fa37d99d07ddd90800

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=libopenssl zlib

PKG_CONFIG_DEPENDS := \
	CONFIG_NGINX_STUB_STATUS \
	CONFIG_NGINX_FLV \
	CONFIG_NGINX_SSL \
	CONFIG_NGINX_DAV \
	CONFIG_NGINX_DISABLE_PCRE \
	CONFIG_NGINX_DISABLE_HTTP_CHARSET_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_GZIP_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_SSI_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_USERID_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_ACCESS_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_AUTH_BASIC_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_AUTOINDEX_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_GEO_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_MAP_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_SPLIT_CLIENTS_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_REFERER_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_REWRITE_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_PROXY_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_FASTCGI_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_UWSGI_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_SCGI_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_MEMCACHED_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_LIMIT_CONN_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_LIMIT_REQ_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_EMPTY_GIF_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_BROWSER_MODULE \
	CONFIG_NGINX_DISABLE_HTTP_UPSTREAM_IP_HASH_MODULE

include $(INCLUDE_DIR)/package.mk

define Package/nginx
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Web Servers/Proxies
  TITLE:=Nginx web server
  URL:=http://nginx.org/
  DEPENDS:=+libpcre
  MENU:=1
endef

define Package/nginx/description
 nginx is an HTTP and reverse proxy server, as well as a mail proxy server,
 written by Igor Sysoev.
endef

define Package/nginx/config
  source "$(SOURCE)/Config.in"
endef

config_files=nginx.conf mime.types fastcgi_params koi-utf koi-win win-utf

define Package/nginx/conffiles
/etc/nginx/
endef

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
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_CACHE),y)
  ADDITIONAL_MODULES += --without-http-cache
endif
ifeq ($(CONFIG_NGINX_DISABLE_PCRE),y)
  ADDITIONAL_MODULES += --without-pcre
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_CHARSET_MODULE),y)
  ADDITIONAL_MODULES += --without-http_charset_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_GZIP_MODULE),y)
  ADDITIONAL_MODULES += --without-http_gzip_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_SSI_MODULE),y)
  ADDITIONAL_MODULES += --without-http_ssi_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_USERID_MODULE),y)
  ADDITIONAL_MODULES += --without-http_userid_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_ACCESS_MODULE),y)
  ADDITIONAL_MODULES += --without-http_access_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_AUTH_BASIC_MODULE),y)
  ADDITIONAL_MODULES += --without-http_auth_basic_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_AUTOINDEX_MODULE),y)
  ADDITIONAL_MODULES += --without-http_autoindex_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_GEO_MODULE),y)
  ADDITIONAL_MODULES += --without-http_geo_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_MAP_MODULE),y)
  ADDITIONAL_MODULES += --without-http_map_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_SPLIT_CLIENTS_MODULE),y)
  ADDITIONAL_MODULES += --without-http_split_clients_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_REFERER_MODULE),y)
  ADDITIONAL_MODULES += --without-http_referer_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_REWRITE_MODULE),y)
  ADDITIONAL_MODULES += --without-http_rewrite_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_PROXY_MODULE),y)
  ADDITIONAL_MODULES += --without-http_proxy_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_FASTCGI_MODULE),y)
  ADDITIONAL_MODULES += --without-http_fastcgi_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_UWSGI_MODULE),y)
  ADDITIONAL_MODULES += --without-http_uwsgi_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_SCGI_MODULE),y)
  ADDITIONAL_MODULES += --without-http_scgi_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_MEMCACHED_MODULE),y)
  ADDITIONAL_MODULES += --without-http_memcached_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_LIMIT_CONN_MODULE),y)
  ADDITIONAL_MODULES += --without-http_limit_conn_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_LIMIT_REQ_MODULE),y)
  ADDITIONAL_MODULES += --without-http_limit_req_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_EMPTY_GIF_MODULE),y)
  ADDITIONAL_MODULES += --without-http_empty_gif_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_BROWSER_MODULE),y)
  ADDITIONAL_MODULES += --without-http_browser_module
endif
ifeq ($(CONFIG_NGINX_DISABLE_HTTP_UPSTREAM_IP_HASH_MODULE),y)
  ADDITIONAL_MODULES += --without-http_upstream_ip_hash_module
endif

define Build/Configure
	# TODO: fix --crossbuild
	(cd $(PKG_BUILD_DIR) ;\
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

define Package/nginx/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)$(PREFIX_ZYX)/sbin/nginx $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/nginx
	$(INSTALL_DATA) $(addprefix $(PKG_INSTALL_DIR)$(ROOT_ZYX)/etc/nginx/,$(config_files)) $(1)/etc/nginx/
	# make nginx bind on both IPv4 and IPv6 by default, when available
  ifeq ($(CONFIG_IPV6),y)
	$(SED) 's/listen\( \+\)80;/listen\1[::]:80;/' $(1)/etc/nginx/nginx.conf
  endif
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

$(eval $(call BuildPackage,nginx))
