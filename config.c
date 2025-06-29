#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"


config_t*
config_load()
{
    char *http_port = getenv("SVC_HTTP_PORT");
    char *http_cert_path = getenv("SVC_HTTP_CERT_PATH");
    char *http_key_path = getenv("SVC_HTTP_KEY_PATH");

    config_t *config = malloc(sizeof(config_t));

    if (http_port != NULL) {
        config->http_port = atoi(http_port);
    }
    if (http_cert_path != NULL) {
        config->http_cert_path = malloc(sizeof(char)*strlen(http_cert_path)+1);
        strcpy(config->http_cert_path, http_cert_path);
    }

    return config;
}

void
config_free(config_t *config)
{
    if (config->http_cert_path != NULL) {
        free(config->http_cert_path);
    }
    if (config->http_key_path != NULL) {
        free(config->http_key_path);
    }

    free(config);
}

void
config_print(const config_t *config)
{
    printf("http_port - %d\n", config->http_port);
    printf("http_cert_path - %s\n", config->http_cert_path);
    printf("http_key_path - %s\n", config->http_key_path);
}
