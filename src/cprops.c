#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cprops.h"

#define WRITE_CHAR(ch)                                                         \
    if (afterEquals)                                                           \
        isValue = true;                                                        \
                                                                               \
    if (isValue)                                                               \
        value_buf[value_i++] = ch;                                             \
    else                                                                       \
        key_buf[key_i++] = ch;

#define CHECK_IF_ESCAPED                                                       \
    if (escape) {                                                              \
        WRITE_CHAR(ch);                                                        \
        escape = false;                                                        \
    }
#define BUFMAX 1024

static int cprops_parse_till_line(char *data, struct cprops_data *fill) {
    bool isValue = false, afterEquals = false, toExit = false, escape = false;
    char value_buf[BUFMAX], key_buf[BUFMAX];
    size_t value_i = 0, key_i = 0;
    char ch;

    while ((ch = *data++) && !toExit) {
        if (((value_i + 1) > BUFMAX) || ((key_i + 1) > BUFMAX))
            return CPROPS_BUFOFLOW;

        switch (ch) {
        // escape the next char
        case '\\':
            CHECK_IF_ESCAPED
            else escape = true;

            break;
        // ignore comments
        case '!':
        case '#':
            CHECK_IF_ESCAPED;

            // comments span the whole line
            while ((ch = *data++))
                // didnt write to *fill
                return 0;
            break;
        case ':':
        case '=':
            if (!afterEquals) {
                if (!escape)
                    afterEquals = true;
                else {
                    escape = false;
                    WRITE_CHAR(ch);
                }
            } else {
                WRITE_CHAR(ch);
            }

            break;
        case '\r':
        case '\n':
            afterEquals = false;
            isValue = false;
            // code should exit parsing from here
            toExit = true;
            break;
        case '\t':
            break;
        // keep whitespace on value data
        case ' ':
            CHECK_IF_ESCAPED;

            if (isValue)
                value_buf[value_i++] = ch;
            break;

        default:
            WRITE_CHAR(ch);
            break;
        }
    }

    // nothing written
    if (value_i == 0 && key_i == 0)
        return 0;

    // +1 to account for '\0'
    fill->key = calloc(sizeof(char), key_i + 1);
    fill->value = calloc(sizeof(char), value_i + 1);

    if (fill->key == NULL || fill->value == NULL)
        return CPROPS_NOMEM;

    strncpy(fill->key, key_buf, key_i);
    strncpy(fill->value, value_buf, value_i);

    return 1;
}

size_t cprops_file_parse(FILE *fp, cprops_data_t *data, int *err,
                         enum cprops_parse_flags flag) {
    char buf[BUFMAX];
    void *ptr = NULL;
    size_t i = 0;
    int ret = 0;
    // init err to 0.
    *err = 0;

    if (!(flag == CPROPS_PARSE_ALLOW_ALLOC || flag == CPROPS_PARSE_DEFAULT))
        goto err_invarg;

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (data->elements_len <= i) {
            if (flag == CPROPS_PARSE_DEFAULT) {
                goto err_nomem;
            } else if (flag == CPROPS_PARSE_ALLOW_ALLOC) {
                ptr = calloc(1, sizeof(struct cprops_data));
                if (ptr == NULL)
                    goto err_nomem;

                data->elements[data->elements_len++] = ptr;
            }
        } else if (*err) {
            return i;
        }

        ret = cprops_parse_till_line(buf, data->elements[i++]);
        // error.
        if (ret < 0)
            *err = ret;
        else if (ret == 0)
            i--;
    }

    return i;

err_nomem:
    *err = CPROPS_NOMEM;
    return i;

err_invarg:
    *err = CPROPS_INVARG;
    return i;
}
