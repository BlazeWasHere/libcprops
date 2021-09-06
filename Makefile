CFLAGS ?= -O3 --std=c99 -D_FORTIFY_SOURCE=2 -fstack-clash-protection -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef
LDFLAGS ?= -shared -fPIC

LN ?= ln
SED ?= sed
SRC := src
OBJ := object
INC := include
EXAMPLE := example
CPROPS_FILE = $(SRC)/cprops.c
CPROPS_HEADER = $(INC)/cprops.h

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

LIBVER_MAJOR_SCRIPT:=`$(SED) -n '/define CPROPS_VERSION_MAJOR/s/.*[[:blank:]]\([0-9][0-9]*\).*/\1/p' < $(CPROPS_HEADER)`
LIBVER_MINOR_SCRIPT:=`$(SED) -n '/define CPROPS_VERSION_MINOR/s/.*[[:blank:]]\([0-9][0-9]*\).*/\1/p' < $(CPROPS_HEADER)`
LIBVER_PATCH_SCRIPT:=`$(SED) -n '/define CPROPS_VERSION_PATCH/s/.*[[:blank:]]\([0-9][0-9]*\).*/\1/p' < $(CPROPS_HEADER)`
LIBVER_MAJOR := $(shell echo $(LIBVER_MAJOR_SCRIPT))
LIBVER_MINOR := $(shell echo $(LIBVER_MINOR_SCRIPT))
LIBVER_PATCH := $(shell echo $(LIBVER_PATCH_SCRIPT))
LIBVER := $(LIBVER_MAJOR).$(LIBVER_MINOR).$(LIBVER_PATCH)

SONAME_FLAGS = -Wl,-soname=libcprops.so.$(LIBVER_MAJOR)
SHARED_EXT_MAJOR = so.$(LIBVER_MAJOR)
SHARED_EXT_VER = so.$(LIBVER)

LIBCPROPS = libcprops.$(SHARED_EXT_VER)

all: $(LIBCPROPS) $(EXAMPLE)

$(LIBCPROPS): $(CPROPS_FILE)
	$(CC) -I$(INC) $^ $(CFLAGS) $(LDFLAGS) $(SONAME_FLAGS) -o $@
	$(LN) -sf $@ libcprops.$(SHARED_EXT_MAJOR)
	$(LN) -sf $@ libcprops.so

$(EXAMPLE): $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) -I$(INC) -c $< -o $@ $(CFLAGS)

#tests:
#	$(MAKE) -C tests

clean:
	@rm -rf $(OBJ)
	@rm -f libcprops.$(SHARED_EXT_MAJOR)
	@rm -f libcprops.so
	@rm -f $(LIBCPROPS)
	@rm -f $(EXAMPLE)
