#ifndef LIBCPROPS_CPROPS_H
#define LIBCPROPS_CPROPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>

#define CPROPS_VERSION_MAJOR 0
#define CPROPS_VERSION_MINOR 0
#define CPROPS_VERSION_PATCH 1
#define CPROPS_VERSION_NUMBER                                                  \
    (CPROPS_VERSION_MAJOR * 100 * 100 + CPROPS_VERSION_MINOR * 100 +           \
     CPROPS_VERSION_PATCH)

struct cprops_data {
    char *key;
    /* alias: property */
    char *value;
};

typedef struct {
    size_t elements_len;
    struct cprops_data **elements;
} cprops_data_t;

enum cprops_err {
    /*
     * An invalid arguement was given to cprops.
     */
    CPROPS_INVARG = -1,
    /*
     * No memory available or unable to allocate anymore memory.
     * Often caused by a failed alloc.
     **/
    CPROPS_NOMEM = -2,
    /*
     * Failed to parse, maybe not enough data fed to cprops
     *
     * There should be 1 key and 1 value per line in the properties file.
     */
    CPROPS_PARSERR = -3,
    /*
     * There would have been a (buffer) overflow if libcprops had continued.
     *
     * This may have been due to extremely large key/value in the
     * properties file.
     */
    CPROPS_BUFOFLOW = -4,
};

enum cprops_parse_flags {
    /*
     * Cprops will return `CPROPS_NOMEM` when there is no more space in
     * `data->elements` accoding to `data->elements_len`.
     */
    CPROPS_PARSE_DEFAULT,
    /*
     * The same as DEFAULT mode except when there is no more space in
     * `data->elements` according to `data->elements_len`, cprops will allocate
     * more memory.
     *
     * The freeing of the new alloc'd data is the user's
     * responsibility.
     */
    CPROPS_PARSE_ALLOW_ALLOC,
};

/*
 * Function returns a size_t of how many elements were written in `*data`.
 *
 * If there was an error, `*err` will be a non zero value filled with a value
 * from `enum cprops_err`.
 */
size_t cprops_file_parse(FILE *fp, cprops_data_t *data, int *err,
                         enum cprops_parse_flags flag);

#ifdef __cplusplus
}
#endif

#endif /* LIBCPROPS_CPROPS_H */
