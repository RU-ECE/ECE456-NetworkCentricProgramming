#ifndef HEADER_CURL_URL_H
#define HEADER_CURL_URL_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/
#include "curl_setup.h"

/*
 * Prototypes for library-wide functions provided by url.c
 */

CURLcode Curl_init_do(struct Curl_easy* data, struct connectdata* conn);
CURLcode Curl_open(Curl_easy** curl);
CURLcode Curl_init_userdefined(Curl_easy* data);

void Curl_freeset(Curl_easy* data);
CURLcode Curl_uc_to_curlcode(CURLUcode uc);
CURLcode Curl_close(Curl_easy** datap); /* opposite of curl_open() */
CURLcode Curl_connect(Curl_easy*, bool* async, bool* protocol_connect);
void Curl_disconnect(Curl_easy* data, connectdata*, bool dead_connection);
CURLcode Curl_setup_conn(Curl_easy* data, bool* protocol_done);
void Curl_free_request_state(Curl_easy* data);
CURLcode Curl_parse_login_details(const char* login, const size_t len, char** userptr, char** passwdptr,
								  char** optionsptr);

/* Get protocol handler for a URI scheme
 * @param scheme URI scheme, case-insensitive
 * @return NULL of handler not found
 */
const struct Curl_handler* Curl_get_scheme_handler(const char* scheme);
const Curl_handler* Curl_getn_scheme_handler(const char* scheme, size_t len);

#define CURL_DEFAULT_PROXY_PORT 1080 /* default proxy port unless specified */
#define CURL_DEFAULT_HTTPS_PROXY_PORT                                                                                  \
	443 /* default https proxy port unless                                                                             \
		   specified */

#ifdef CURL_DISABLE_VERBOSE_STRINGS
#define Curl_verboseconnect(x, y) Curl_nop_stmt
#else
void Curl_verboseconnect(Curl_easy* data, connectdata* conn);
#endif

#if defined(USE_HTTP2) || defined(USE_HTTP3)
void Curl_data_priority_clear_state(struct Curl_easy* data);
#else
#define Curl_data_priority_clear_state(x)
#endif /* !(defined(USE_HTTP2) || defined(USE_HTTP3)) */

#ifdef USE_NGHTTP2
CURLcode Curl_data_priority_add_child(struct Curl_easy* parent, struct Curl_easy* child, bool exclusive);
#else
#define Curl_data_priority_add_child(x, y, z) CURLE_NOT_BUILT_IN
#endif

#endif /* HEADER_CURL_URL_H */
