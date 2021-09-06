# libcprops

A minimal [properties](https://en.wikipedia.org/wiki/.properties) file parser.

# Features
- compatible with C99
- no dependencies
- ~100 LOC
- simple API
- no memory leaks (tested with [valgrind](https://www.valgrind.org/))
- no overflows (tested with [valgrind](https://www.valgrind.org/))
- shared library

# API
```c
size_t cprops_file_parse(FILE *fp, cprops_data_t *data, int *err,
                         enum cprops_parse_flags flag);
```

# Usage
View [example.c](https://github.com/blazewashere/libcprops/blob/main/src/example.c).

# Example
```sh
$ make example
[...]
$ ./example
written: 9, errno: 0
file close ret: 0
key: website, value: https://en.wikipedia.org/
key: language, value: English
key: message, value: Welcome to 
key: key, value: valueOverOneLine\
key: key with spaces, value: This is the value that could be looked up with the key "key with spaces".
key: key:with=colonAndEqualsSign, value: This is the value for the key "key:with=colonAndEqualsSign"
key: tab, value: u0009
key: path, value: c:\wiki\templates
key: escape, value: back\nslash
key: (null), value: (null)
```

# Shared Library
To build the shared library run:
```sh
$ make
[...]
$ ls
example  libcprops.so    libcprops.so.0.0.1  object     src
include  libcprops.so.0  Makefile            README.md  tests
```

# TODO
- Tests
- Parse multiline properties e.g
```properties
message = Welcome to \
          Wikipedia!
```

# License
[BSL-1.0 License](https://github.com/BlazeWasHere/libcprops/blob/main/LICENSE)
