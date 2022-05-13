CC     = gcc
CFLAGS = -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

DBG = -g

SHA_LIB = ~/Programming/SHA_256/sha_256.a
MY_LIB  = ~/Programming/My_Lib/My_Lib.a

BIN      = ./bin/
SRCDIR   = ./src/
BUILDDIR = ./build/

SRC_LIST = main.c Hash_Table.c Hash_Table_Dump.c Hash_Research.c
SRC = $(addprefix $(SRCDIR),$(SRC_LIST))

SUBS := $(SRC)
SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(SUBS))

OBJ  = $(SUBS:.c=.o)
DEPS = $(SUBS:.c=.d)

include $(DEPS)

.PHONY: all

all: $(DEPS) $(OBJ)
	@mkdir -p $(BIN)
	@echo "Linking project..."
	@$(CC) $(OBJ) $(SHA_LIB) $(MY_LIB) -o $(BIN)Hash_Table.out

$(BUILDDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling \"$<\"..."
	@$(CC) $(CFLAGS) $(DBG) $(DEBUG) -c $< -o $@

$(BUILDDIR)%.d: $(SRCDIR)%.c
	@echo "Collecting dependencies for \"$<\"..."
	@mkdir -p $(dir $@)
	@$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@	

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
