CC     = gcc
CFLAGS = -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

BIN      = ./bin/
SRCDIR   = ./src/
BUILDDIR = ./build/

SRC_LIST = main.c Hash_Table.c Fill_Hash_Table.c
SRC = $(addprefix $(SRCDIR),$(SRC_LIST))

SUBS := $(SRC)
SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(SUBS))

OBJ  = $(SUBS:.c=.o)
DEPS = $(SUBS:.c=.d)

LIBS_LIST = My_Lib
LIBSDIR = $(addprefix ../../lib/, $(LIBS_LIST))
LIBS = $(addsuffix /*.a, $(LIBSDIR))
FIND = $(addprefix -I, $(LIBSDIR))

.PHONY: all $(LIBSDIR)

all: $(DEPS) $(OBJ) $(LIBSDIR)
	@mkdir -p $(BIN)
	@echo "Linking project..."
	@$(CC) $(OBJ) $(LIBS) -o $(BIN)Hash_Table.out

$(LIBSDIR):
	@$(MAKE) -C $@ --no-print-directory -f Makefile.mak

$(BUILDDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling \"$<\"..."
	@$(CC) $(CFLAGS) $(FIND) -c -g $(OPT) $< -o $@

include $(DEPS)

$(BUILDDIR)%.d: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Collecting dependencies for \"$<\"..."
	@$(CC) -E $(CFLAGS) $(FIND) $< -MM -MT $(@:.d=.o) > $@	

.PHONY: clean run profile

run: $(BIN)Hash_Table.out
	@echo "Running \"$<\"..."
	@$(BIN)Hash_Table.out

clean:
	rm -rf $(OBJ) $(DEPS)

profile: $(BIN)Hash_Table.out
	@echo "Profiling \"$<\"..."
	@valgrind --tool=callgrind $(BIN)Hash_Table.out
	@kcachegrind
	@rm -rf callgrind.out.*
