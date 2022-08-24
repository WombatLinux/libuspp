//
// Created by afroraydude on 8/14/2021.
//

#include <cjson/cJSON.h>

#ifndef USPM_REPO_H
#define USPM_REPO_H

#endif //USPM_REPO_H

/**
 * Gets the package.json file from a mirror [url]
 *
 * @param url the repo
 */
cJSON *get_repo_json(char* url);


/**
 * Downloads a package [package] file from a mirror [mirror]
 *
 *
 * @param mirrors the web mirrors to load from
 * @param package the package
 */
int download_package(cJSON *mirrors, char *package);