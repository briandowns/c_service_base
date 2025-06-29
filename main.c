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
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>

#include <ulfius.h>

#include "api.h"
#include "config.h"
#include "http.h"
#include "logger.h"

#define STR1(x) #x
#define STR(x) STR1(x)

config_t *config = NULL;

static const char* usage = "usage: service\n"
"\nENV Vars:\n"
"  SVC_HTTP_PORT (required)\n"
"  SVC_HTTP_PORT\n";

static void
cleanup()
{
    config_free(config);
}

void
sig_handler(int sig)
{
    s_log(S_LOG_INFO, s_log_string("msg", "received signal"),
        s_log_int("signal", sig));
    cleanup();

    exit(1);
}

int
main(int argc, char **argv)
{
signal(SIGINT, sig_handler);

    s_log_init(stdout);
    
    int c;
    if (argc > 1) {
        while ((c = getopt(argc, argv, "h")) != -1) {
            switch (c) {
                case 'h':
                    printf("%s\n", usage);
                    return EXIT_SUCCESS;
                default:
                    abort();
            }
        }
    }

    s_log(S_LOG_INFO, s_log_string("msg", "starting service..."));

    config = config_load();
    s_log(S_LOG_INFO, s_log_string("msg", "config loaded..."));

    api_init(config, (const char*)STR(git_sha));
    api_start();

    s_log(S_LOG_INFO, s_log_string("msg", "shutting down thinq..."));

    cleanup();

    return 0;
}
