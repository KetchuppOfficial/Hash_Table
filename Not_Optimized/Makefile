CC     = gcc
CFLAGS = -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

DBG = -g

BIN      = ./bin/
SRCDIR   = ./src/
BUILDDIR = ./build/

SRC_LIST = main.c Hash_Table.c Hash_Table_Dump.c Hash_Research.c
SRC = $(addprefix $(SRCDIR),$(SRC_LIST))

SUBS := $(SRC)
SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(SUBS))

OBJ  = $(SUBS:.c=.o)
DEPS = $(SUBS:.c=.d)

LIBS_LIST = My_Lib SHA_256
LIBSDIR = $(addprefix ../lib/, $(LIBS_LIST))
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
	@$(CC) $(CFLAGS) $(FIND) $(DBG) $(OPT) -c $< -o $@

include $(DEPS)

$(BUILDDIR)%.d: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Collecting dependencies for \"$<\"..."
	@$(CC) -E $(CFLAGS) $(FIND) $< -MM -MT $(@:.d=.o) > $@	

.PHONY: clean run

run: $(BIN)Hash_Table.out
	@echo "Running \"$<\"..."
	@$(BIN)Hash_Table.out

clean:
	rm -rf $(OBJ) $(DEPS)
