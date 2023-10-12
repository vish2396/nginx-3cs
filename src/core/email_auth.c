#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
    ngx_str_t file;
} ngx_http_email_auth_conf_t;

static ngx_int_t ngx_http_email_auth_init(ngx_conf_t *cf) {
    return NGX_OK;
}

static ngx_int_t ngx_http_email_auth_handler(ngx_http_request_t *r) {
    ngx_http_email_auth_conf_t *conf = ngx_http_get_module_loc_conf(r, ngx_http_email_auth_module);

    ngx_str_t header;
    header.data = (u_char *) "X-Email";
    header.len = sizeof("X-Email") - 1;

    ngx_str_t value;
    value.data = ngx_http_map_find(r->headers_in, header, &value.len);

    if (value.data == NULL) {
        return NGX_HTTP_FORBIDDEN;
    }

    ngx_fd_t fd;
    u_char *buf;

    fd = ngx_open_file(conf->file.data, NGX_FILE_RDONLY, 0644);
    if (fd == NGX_INVALID_FILE) {
        return NGX_HTTP_FORBIDDEN;
    }

    buf = ngx_palloc(r->pool, value.len + 1);
    if (buf == NULL) {
        ngx_close_file(fd);
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_read_file(fd, buf, value.len);
    buf[value.len] = '\0';

    ngx_close_file(fd);

    if (ngx_strcmp(buf, value.data) != 0) {
        return NGX_HTTP_FORBIDDEN;
    }

    return NGX_OK;
}

static ngx_command_t ngx_http_email_auth_commands[] = {
    { ngx_string("email_auth"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_email_auth_conf_t, file),
      NULL },

    ngx_null_command
};

static ngx_http_module_t ngx_http_email_auth_module = {
    NGX_MODULE_V1,
    ngx_http_email_auth_init,
    NULL,
    NULL,
    NULL,
    NULL,
    ngx_http_email_auth_commands,
    NGX_HTTP_MODULE_LOC_CONF|NGX_HTTP_MODULE_SLOT,
    NULL,
    NULL
};

ngx_module_t ngx_http_email_auth_module_ctx = {
    NGX_MODULE_V1,
    &ngx_http_email_auth_module
};
