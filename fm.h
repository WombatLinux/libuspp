/*
 * fm - manages the json files and stuff
 * few extras added
 * HEADER
 */

#ifndef USPM_FM_H
#define USPM_FM_H

#include <cjson/cJSON.h>
#include <stddef.h>
#include <stdio.h>

/**
 * concatenate strings
 *
 * @param s1 first string
 * @param s2 second string
 * */
char* concat(const char *s1, const char *s2);

/**
 * Writes a fresh new packages file
 *
 * @param out the char array to output to the file
 */
void write_packages_file(char *out);

/**
 * Writes a fresh new config file
 *
 * @param out the char array to output to the file
 */
void write_config_file(char *out);

/**
 * Adds a package [packagename] to the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 * @param packagename the Package
 * @param packagedata the internal data
 */
int add_to_packages(char *packagename, cJSON *packagedata);

/**
 * Removes a package [packagename] from the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 *
 * @param packagename package to remove
 */
int remove_from_packages(char *packagename);

/**
 * Checks for the existence of a config file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_config_file();

/**
 * Checks for the existence of a packages file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_packages_file();

/**
 * Performs a file [file] checksum and outputs the value to a char array [o]
 *
 * @param filename the file
 * @param o the char array to output the data to
 */
void checksum(char *filename, char *o[16]);

/**
 * Given two [a] and [b] checksums, compares them
 *
 * @param a first file's checksum
 * @param b second file's checksum
 */
int checksum_compare(char *a, char *b);

/**
 * Verifies the checksum of a package [package] downloaded with the checksum of the
 * mirror [mirror]
 *
 * @param package the package
 * @param mirror the repo to pull from
 */
int verify_checksum(char *mirror, char *package);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

#endif //USPM_FM_H
