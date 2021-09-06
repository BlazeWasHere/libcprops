#include <err.h>
#include <stdlib.h>

#include "cprops.h"

#define PATH "tests/data/5.properties"

int main(void) {
    int errn, ret;

    FILE *fp = fopen(PATH, "r");
    if (fp == NULL) {
        err(EXIT_FAILURE, "error opening %s", PATH);
    }

    cprops_data_t *data = calloc(1, sizeof(cprops_data_t));

    // ideally you would handle any alloc failues
    // allocate enough for `data->elements_len` pointers (void *)
    data->elements_len = 10;
    data->elements = calloc(data->elements_len, sizeof(void *));

    for (size_t i = 0; i < data->elements_len; i++)
        // ditto.
        data->elements[i] = calloc(1, sizeof(struct cprops_data));

    ret = cprops_file_parse(fp, data, &errn, CPROPS_PARSE_DEFAULT);
    printf("written: %d, errno: %d\n", ret, errn);

    // ideally you would handle any possible errors
    ret = fclose(fp);
    printf("file close ret: %d\n", ret);

    for (size_t i = 0; i < data->elements_len; i++) {
        printf("key: %s, value: %s\n", data->elements[i]->key,
               data->elements[i]->value);
        free(data->elements[i]->key);
        free(data->elements[i]->value);
        free(data->elements[i]);
    }

    free(data->elements);
    free(data);

    return EXIT_SUCCESS;
}