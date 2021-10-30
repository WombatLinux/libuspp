#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include "build_config.h"
#include "config.h"
#include "fm.h"

/* TODO: Move json files to either etc or outside storage dir */

/**
 * Checks for the existence of a packages file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 * @return 0 if exists 1 if nonexistent
 */
int check_packages_file() {
    if (access("packages.json", F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        return 0;
        // printf("%s\n", json);
    } else {
        create_packages_file();
        return 1;
    }
}

/**
 * creates packages file
 */
void create_packages_file() {
    char *out;
    cJSON *root, *uspm;

    root = cJSON_CreateObject();
    uspm = cJSON_CreateObject();

    printf("Creating packages file with default values\n");

    /* add data to uspm package */
    cJSON_AddItemToObject(uspm, "version", cJSON_CreateString(("1.0.0")));
    cJSON_AddItemToObject(uspm, "dependencies", cJSON_CreateObject());

    /* add data to root object */
    cJSON_AddItemToObject(root, "uspm", uspm);

    out = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    write_packages_file(out);

    free(out);
}

/**
 * Given a filename/file location [file], attempts to parse as JSON
 * and then loads it in as a cJSON object.
 *
 * @param file file to load
 * @returnjson format of file
 */
cJSON *load_file(char *file) {
    if (access(file, FOK) != -1) {
        /* declare a file pointer */
        FILE *infile;
        char *buffer;
        long numbytes;

        /* open an existing file for reading */
        infile = fopen(file, "r");

        /* Get the number of bytes */
        fseek(infile, 0L, SEEK_END);
        numbytes = ftell(infile);

        /* reset the file position indicator to
        the beginning of the file */
        fseek(infile, 0L, SEEK_SET);

        /* grab sufficient memory for the
        buffer to hold the text */
        buffer = (char *) calloc(numbytes, sizeof(char));

        /* memory error */
        if (buffer == NULL)
            return NULL;

        /* copy all the text into the buffer */
        fread(buffer, sizeof(char), numbytes, infile);
        fclose(infile);

        /* confirm we have read the file by
        outputing it to the console */
        //printf("The file contains this text\n\n%s", buffer);

        cJSON *root = load_json(buffer);

        /* free the memory we used for the buffer */
        free(buffer);

        return root;
    } else {
        printf("No file %s\n", file);
        return NULL;
    }
}

void create_config_file() {
    char *out;
    cJSON *root;
    printf("Creating config file with default values\n");
    root = cJSON_CreateObject();

    /* add data to uspm package */
    /* FIXME: Change this to match new stuff */
    cJSON_AddItemToObject(root, "mirror", cJSON_CreateString(("http://repo.wombatlinux.org/")));

    out = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    write_config_file(out);

    free(out);
}

/**
 * Checks for the existence of a config file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_config_file() {
    if (access("config.json", F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        return 0;
        // printf("%s\n", json);
    } else {
        create_config_file();
        return 1;
    }
}

/**
 * Writes a fresh new packages file
 *
 * @param out the char array to output to the file
 */
void write_packages_file(char *out) {
    char *outfile = concat(out, "\n");

    write_file("packages.json", outfile);
}

/**
 * Writes a fresh new config file
 *
 * @param out the char array to output to the file
 */
void write_config_file(char *out) {
    char *outfile = concat(out, "\n");

    write_file("config.json", outfile);
}

/**
 * Removes a package [packagename] from the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 *
 * @param packagename package to remove
 */
int remove_from_packages(char *packagename) {
    cJSON *root = load_file("packages.json");
    cJSON_DeleteItemFromObject(root, packagename);

    char *out = cJSON_Print(root);

    write_packages_file(out);

    free(out);

    return 0;
}

/**
 * Adds a package [packagename] to the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 * @param packagename the Package
 * @param packagedata the internal data
 */
int add_to_packages(char *packagename, cJSON *packagedata) {
    chdir("/var/uspm/storage/");

    cJSON *root = load_file("packages.json");


    cJSON_AddItemToObject(root, packagename, packagedata);

    char *out = cJSON_Print(root);

    write_packages_file(out);

    free(out);

    return 0;
}

