# Custom Nginx Modules with Lua
### Version 1.18.0
Contains All option from official nginx

Contains the following extra nginx and lua 2.1 modules

- Lua-resty-jit 2.1
- ModSecurity Dynamic Module
- Nginx Developer Kit Module
- Lua Nginx Dynamic Module

## Notes

- SpiderLabs/ModSecurity installed.
- Nginx V 1.18.0, if you are not going to install it via this repo
## Installation

## Install openresty lua
```sh 
cd external/lua-resty-jit
make install 
```

## Configure

```sh
LUAJIT_LIB=/usr/local/bin LUAJIT_INC=/usr/local/include/luajit-2.1 ./configure --add-dynamic-module=dynamic/ModSecurity --add-dynamic-module=external/ngx_devel_kit --add-dynamic-module=external/lua_nginx_module --with-compat
```

# Install Nginx (Optional - skip if you already have Nginx )
```sh
make install
```

# Install Modules
```sh
make modules
```

# Install OpenResty Modules

```sh
cd external/lua-resty-core && make install
cd external/lua-resty-redis && make install
```

# Copy Libs
```sh
cp objs/ngx_http_lua_module.so /usr/share/nginx/modules/
cp objs/ndk_http_module.so /usr/share/nginx/modules/
cp objs/ngx_http_modsecurity_module.so /usr/share/nginx/modules
```

## Add lua jit path to /etc/ld.so.conf
```sh
echo "/usr/local/lib" >> /etc/ld.so.conf
```

## Nginx Config

Before event {} block

```conf
load_module modules/ngx_http_modsecurity_module.so;
load_module modules/ndk_http_module.so;
load_module modules/ngx_http_lua_module.so;
```

Inside http {} block

```conf
    modsecurity on;
    modsecurity_rules_file /etc/nginx/modsec/modsec-config.conf;
	lua_package_path "/usr/local/lib/lua/?.lua;;";
```