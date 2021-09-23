//
// Created by afroraydude on 8/14/2021.
//

#include "repo.h"
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <openssl/md5.h>
#include "mem.h"

/**
 * Gets the package.json file from a mirror [url]
 *
 * @param url the repo
 */
cJSON *get_repo_json(char *url) {
    CURL *curl_handle;
    CURLcode res;

    /* 2020-11-07:
    * IDK why this is here, but I am too afraid
    * to break something if I delete this. So it
    * stays in the code
    *
    * - afroraydude
    */
    chdir("adsfijo");

    url = concat(url, "packages.json");

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    } else {
#ifdef _DEBUG_
        printf("%lu bytes retrieved\n", (unsigned long) chunk.size);
        printf("data: %s\n", chunk.memory);
#endif
    }


    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    cJSON *data = cJSON_Parse(chunk.memory);

    free(chunk.memory);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return data;
}