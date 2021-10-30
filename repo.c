//
// Created by afroraydude on 8/14/2021.
//

#include "repo.h"
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "mem.h"
#include "fm.h"

int check_if_package_exists(char *mirror, char *folder, char *package) {
    char *url = concat(mirror, folder);

    cJSON *json = get_repo_json(url);

    cJSON *packagejson = cJSON_GetObjectItem(json, package);

    if (packagejson == NULL) {
        return 1;
    }

    return 0;
}

/**
 * Downloads a package [package] file from a mirror [mirror]
 *
 *
 * @param mirror the web mirror to load from
 * @param package the package
 */
int download_package(char *mirror, char *package) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    int folder = 0;

    char *url = mirror;

    if (check_if_package_exists(mirror, "core/", package) == 0) {
        folder = 1;

    } else if (check_if_package_exists(mirror, "extra/", package) == 0) {
        folder = 2;
    } else if (check_if_package_exists(mirror, "community/", package) == 0) {
        folder = 3;
    }

    switch(folder) {
        case (1): url = concat(url, "core/"); break;
        case (2): url = concat(url, "extra/"); break;
        case (3): url = concat(url, "community/"); break;
        default: return 1; // error
    }

    mirror = url;

    url = concat(url, package);


    url = concat(url, ".uspm");
    printf("%s\n", url);

    char outfilename[FILENAME_MAX] = "";
    strcpy(outfilename, concat(package, ".uspm"));
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            printf("fail");
            return 1;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
        {
            // success do not thing
        }
        else
        {
            printf("Failed to download file\n");
            curl_easy_cleanup(curl);
            return 1;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    free(url);

#ifdef _CHECKSUM_
    if (verify_checksum(mirror, package) != 0) return 1;
#endif
    return 0;
}

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
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/");

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