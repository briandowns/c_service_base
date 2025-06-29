/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#include <jansson.h>
#include <orcania.h>
#include <ulfius.h>

#include "api.h"
#include "config.h"
#include "http.h"
#include "logger.h"

#define DEFAULT_PORT 8080

#define AUTH_HEADER "X-Service-Auth"

#define HEALTH_PATH      "/healthz"
#define API_PATH         "/api/v1"
#define QUEUE_PATH       "/queue"
#define NAMED_QUEUE_PATH "/queue/:name"
#define QUEUE_COUNT_PATH "/queue/:name/count"

#define UNUSED(x) (void)x

/**
 * time_spent takes the start time of a route handler
 * and calculates how long it ran for. It then returns
 * that value to be logged.
 */
#define TIME_SPENT(x)                        \
{                                            \
    clock_t diff = clock() - x;              \
    int msec = diff * 1000 / CLOCKS_PER_SEC; \
    msec % 1000;                             \
}

static struct _u_instance instance;

void
log_request(const struct _u_request *request, struct _u_response *response, clock_t start)
{
    clock_t diff = clock() - start;
    int msec = diff * 1000;

    s_log(S_LOG_INFO,
        s_log_string("method", request->http_verb), 
        s_log_string("path", request->url_path),
        //s_log_string("host", ipv4),
        s_log_uint32("status", response->status),
        s_log_string("proto", request->http_protocol),
        s_log_int("duration", msec),
        s_log_string("client_addr", inet_ntoa(((struct sockaddr_in*)request->client_address)->sin_addr)));
}

/**
 * Callback function for basic authentication
 */
int
callback_auth_basic (const struct _u_request *request, struct _u_response *response, void *user_data)
{
    UNUSED(request);
    UNUSED(user_data);

    ulfius_set_string_body_response(response, 200, "Basic auth callback");
    return U_CALLBACK_CONTINUE;
}

int
callback_auth_token(const struct _u_request *request, struct _u_response *response, void *user_data)
{
    //const char *token = u_map_get(request->map_header, AUTH_HEADER);

    //char *token = u_map_get(request->map_url, AUTH_HEADER);

    return U_CALLBACK_CONTINUE;
}

/**
 * callback_default is used to handled calls that don't have
 * a matching route. Returns an expected 404.
 */
int
callback_default(const struct _u_request *request, struct _u_response *response, void *user_data)
{
    ulfius_set_string_body_response(response, HTTP_STATUS_CODE_NOT_FOUND, "page not found");

    return U_CALLBACK_CONTINUE;
}

/**
 * callback_health_check handles all health check
 * requests to the thinq service.
 */
int
callback_health_check(const struct _u_request *request, struct _u_response *response, void *user_data)
{
    clock_t start = clock();

    const char *git_hash = (const char *)user_data;
    
    json_t * json_body = json_object();
    json_object_set_new(json_body, "status", json_string("OK")); 
    json_object_set_new(json_body, "git_sha", json_string(git_hash));

    ulfius_set_json_body_response(response, HTTP_STATUS_CODE_OK, json_body);
    json_decref(json_body);

    log_request(request, response, start);

    return U_CALLBACK_CONTINUE;
}

/**
 * callback_hello handles requests for hello.
 */
int
callback_hello(const struct _u_request *request, struct _u_response *response, void *user_data)
{
    clock_t start = clock();

    json_t *json_body = json_object();
    json_object_set_new(json_body, "hello", json_string("world"));
    ulfius_set_json_body_response(response, 200, json_body);
    json_decref(json_body);

    log_request(request, response, start);

    return U_CALLBACK_CONTINUE;
}


int
api_init(config_t *config,   const char *git_hash)
{
    if (ulfius_init_instance(&instance, config->http_port, NULL, NULL) != U_OK) {
        fprintf(stderr, "error ulfius_init_instance, abort\n");
        return EXIT_FAILURE;
    }

    ulfius_add_endpoint_by_val(&instance, HTTP_METHOD_GET, HEALTH_PATH, NULL, 0, &callback_health_check, (void*)git_hash);

    ulfius_add_endpoint_by_val(&instance, HTTP_METHOD_GET, API_PATH, NAMED_QUEUE_PATH, 0, &callback_hello, NULL);

    ulfius_set_default_endpoint(&instance, &callback_default, NULL);

    return 0;
}

void
api_start()
{
    if (ulfius_start_framework(&instance) == U_OK) {
        s_log(S_LOG_INFO, s_log_string("msg", "starting thinq api server"), s_log_int("port", instance.port));

        getchar();
    } else {
        s_log(S_LOG_ERROR, s_log_string("msg", "error starting server"));
    }

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
}