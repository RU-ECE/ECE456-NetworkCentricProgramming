#ifndef HEADER_CURL_SPLAY_H
#define HEADER_CURL_SPLAY_H
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
#include "timeval.h"

struct Curl_tree {
	Curl_tree *smaller; /* smaller node */
	Curl_tree *larger;  /* larger node */
	Curl_tree *samen;   /* points to the next node with identical key */
	Curl_tree *samep;   /* points to the prev node with identical key */
	curltime key;        /* this node's "sort" key */
  void *payload;             /* data the splay code doesn't care about */
};

Curl_tree *Curl_splay(curltime i, Curl_tree *t);

Curl_tree *Curl_splayinsert(curltime key, Curl_tree *t, Curl_tree *newnode);

Curl_tree *Curl_splaygetbest(curltime key, Curl_tree *t, Curl_tree **removed);

int Curl_splayremove(Curl_tree *t, Curl_tree *removenode, Curl_tree **newroot);

#define Curl_splaycomparekeys(i,j) ( ((i.tv_sec)  < (j.tv_sec)) ? -1 : \
                                   ( ((i.tv_sec)  > (j.tv_sec)) ?  1 : \
                                   ( ((i.tv_usec) < (j.tv_usec)) ? -1 : \
                                   ( ((i.tv_usec) > (j.tv_usec)) ?  1 : 0))))

#endif /* HEADER_CURL_SPLAY_H */
