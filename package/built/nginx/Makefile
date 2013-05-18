#
# Copyright (C) 2009-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=nginx
PKG_VERSION:=1.4.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://nginx.org/download/
PKG_MD5SUM:=fea7dfab995545ce27fe4c49dc21a972

################################################################################################

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

################################################################################################

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=zlib

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
	CONFIG_NGINX_HTTP_UPSTREAM_IP_HASH \
	CONFIG_NGINX_SPDY \
	CONFIG_NGINX_REALIP \
	CONFIG_NGINX_ADDITION \
	CONFIG_NGINX_XSLT \
	CONFIG_NGINX_IMAGE_FILTER \
	CONFIG_NGINX_GEOIP \
	CONFIG_NGINX_SUB \
	CONFIG_NGINX_MP4 \
	CONFIG_NGINX_GUNZIP \
	CONFIG_NGINX_GZIP_STATIC \
	CONFIG_NGINX_RANDOM_INDEX \
	CONFIG_NGINX_SECURE_LINK \
	CONFIG_NGINX_DEGRADATION \
	CONFIG_NGINX_PERL \
	CONFIG_NGINX_GOOGLE_PERFTOOLS \
	CONFIG_NGINX_HTTP_UPSTREAM_LEAST_CONN \
	CONFIG_NGINX_HTTP_UPSTREAM_KEEPALIVE

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
        TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib
endef

# FIXME: +NGINX_XSLT:libxslt, +NGINX_IMAGE_FILTER:libgd - nginx don't see libraries
Package/nginx-basic=$(call Package/nginx/Default,basic,0,+libpcre +libopenssl)
Package/nginx-custom=$(call Package/nginx/Default,custom,1,+NGINX_PCRE:libpcre +NGINX_LUA:liblua +NGINX_SSL:libopenssl)
Package/nginx-full=$(call Package/nginx/Default,full,0,+libpcre +liblua +libgd +libopenssl)

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

CONFIG_NGINX_SPDY:=n
CONFIG_NGINX_REALIP:=n
CONFIG_NGINX_ADDITION:=n
CONFIG_NGINX_IMAGE_FILTER:=n
CONFIG_NGINX_SUB:=n
CONFIG_NGINX_MP4:=n
CONFIG_NGINX_GUNZIP:=n
CONFIG_NGINX_GZIP_STATIC:=n
CONFIG_NGINX_RANDOM_INDEX:=n
CONFIG_NGINX_SECURE_LINK:=n
CONFIG_NGINX_DEGRADATION:=n

CONFIG_NGINX_HTTP_UPSTREAM_LEAST_CONN:=y
CONFIG_NGINX_HTTP_UPSTREAM_KEEPALIVE:=y

CONFIG_NGINX_XSLT:=n

CONFIG_NGINX_GEOIP:=n
CONFIG_NGINX_GOOGLE_PERFTOOLS:=n

CONFIG_NGINX_PERL:=n
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

CONFIG_NGINX_REALIP:=y
CONFIG_NGINX_ADDITION:=y
CONFIG_NGINX_IMAGE_FILTER:=y
CONFIG_NGINX_SUB:=y
CONFIG_NGINX_MP4:=y
CONFIG_NGINX_GUNZIP:=y
CONFIG_NGINX_GZIP_STATIC:=y
CONFIG_NGINX_RANDOM_INDEX:=y
CONFIG_NGINX_SECURE_LINK:=y
CONFIG_NGINX_DEGRADATION:=y

CONFIG_NGINX_HTTP_UPSTREAM_LEAST_CONN:=y
CONFIG_NGINX_HTTP_UPSTREAM_KEEPALIVE:=y

# OpenSSL 1.0.1 required!
CONFIG_NGINX_SPDY:=n

# Bug: failed to detect libraries. FIXME!!!
CONFIG_NGINX_XSLT:=n

# LibGeoIP not in repo
CONFIG_NGINX_GEOIP:=n
CONFIG_NGINX_GOOGLE_PERFTOOLS:=n

# TODO: разобраться, какого хрена warning'и обрывают сборку. И починить, ессно, чтобы без Warning'ов собиралось.
CONFIG_NGINX_PERL:=n
endif

################################################################################################

config_files=nginx.conf mime.types fastcgi_params koi-utf koi-win win-utf

################################################################################################

define Package/nginx-$(BUILD_VARIANT)/conffiles
/etc/nginx/
endef

################################################################################################
################################################################################################

ADDITIONAL_MODULES:=

#### 1.2.x config here:

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
# WARNING!!! External files required...
	ADDITIONAL_MODULES += --add-module=$(PKG_BUILD_DIR)/lua-nginx
endif
ifeq ($(CONFIG_NGINX_RTMP),y)
# WARNING!!! External files required...
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

#### 1.4.0 modules here:

ifeq ($(CONFIG_NGINX_SPDY),y)
# WARNING!!! OpenSSL 1.0.1 required!
	ADDITIONAL_MODULES += --with-http_spdy_module
endif
ifeq ($(CONFIG_NGINX_REALIP),y)
	ADDITIONAL_MODULES += --with-http_realip_module
endif
ifeq ($(CONFIG_NGINX_ADDITION),y)
	ADDITIONAL_MODULES += --with-http_addition_module
endif
ifeq ($(CONFIG_NGINX_XSLT),y)
	ADDITIONAL_MODULES += --with-http_xslt_module
endif
ifeq ($(CONFIG_NGINX_IMAGE_FILTER),y)
	ADDITIONAL_MODULES += --with-http_image_filter_module
endif
ifeq ($(CONFIG_NGINX_GEOIP),y)
	ADDITIONAL_MODULES += --with-http_geoip_module
endif
ifeq ($(CONFIG_NGINX_SUB),y)
	ADDITIONAL_MODULES += --with-http_sub_module
endif
ifeq ($(CONFIG_NGINX_MP4),y)
	ADDITIONAL_MODULES += --with-http_mp4_module
endif
ifeq ($(CONFIG_NGINX_GUNZIP),y)
	ADDITIONAL_MODULES += --with-http_gunzip_module
endif
ifeq ($(CONFIG_NGINX_GZIP_STATIC),y)
	ADDITIONAL_MODULES += --with-http_gzip_static_module
endif
ifeq ($(CONFIG_NGINX_RANDOM_INDEX),y)
	ADDITIONAL_MODULES += --with-http_random_index_module
endif
ifeq ($(CONFIG_NGINX_SECURE_LINK),y)
	ADDITIONAL_MODULES += --with-http_secure_link_module
endif
ifeq ($(CONFIG_NGINX_DEGRADATION),y)
	ADDITIONAL_MODULES += --with-http_degradation_module
endif
ifeq ($(CONFIG_NGINX_PERL),y)
# WARNING!!! External binary & libs required... (possibly)
	ADDITIONAL_MODULES += --with-http_perl_module
endif
ifeq ($(CONFIG_NGINX_GOOGLE_PERFTOOLS),y)
	ADDITIONAL_MODULES += --with-google_perftools_module
endif

#### 1.4.0 "http disables" here:

ifneq ($(CONFIG_NGINX_HTTP_UPSTREAM_LEAST_CONN),y)
	ADDITIONAL_MODULES += --without-http_upstream_least_conn_module
endif
ifneq ($(CONFIG_NGINX_HTTP_UPSTREAM_KEEPALIVE),y)
	ADDITIONAL_MODULES += --without-http_upstream_keepalive_module
endif

################################################################################################
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
	VERSION:=c558c4347b289ac96e8ad09501629f79251b3911
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
	VERSION:=c61e99a36878fc618e29633db0e9652be13f6284
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
