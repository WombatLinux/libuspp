#ifndef USPM_CONFIG_H
#define USPM_CONFIG_H

/**
 * Creates package json file
 */
void create_packages_file();

/**
 * Creates package json file
 */
void create_config_file();

/**
 * Checks for the existence of a config file
 *
 * @return 1 if nonexistent 0 if exists
 */
int check_config_file();

/**
 * Checks for the existence of a packages file
 * @return 1 if nonexistent 0 if exists
 */
int check_packages_file();

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
 * Given a filename/file location [file], attempts to parse as JSON
 * and then loads it in as a cJSON object.
 *
 * @param file file to load
 * @return file in JSON format
 */
cJSON *load_file(char *file);

#endif