CC     = gcc
CFLAGS = -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

DBG = -g

SHA_LIB_PATH = /home/ketchupp/Programming/SHA_256/
MY_LIB_PATH  = /home/ketchupp/Programming/My_Lib/

SHA_LIB = $(join $(SHA_LIB_PATH), sha_256.a)
MY_LIB  = $(join $(MY_LIB_PATH), My_Lib.a)

BIN      = ./bin/
SRCDIR   = ./src/
BUILDDIR = ./build/

SRC_LIST = main.c Hash_Table.c Hash_Table_Dump.c Hash_Research.c
SRC = $(addprefix $(SRCDIR),$(SRC_LIST))

SUBS := $(SRC)
SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(SUBS))

OBJ  = $(SUBS:.c=.o)
DEPS = $(SUBS:.c=.d)

.PHONY: all

all: $(DEPS) $(OBJ)
	@mkdir -p $(BIN)
	@echo "Linking project..."
	@$(CC) $(OBJ) $(SHA_LIB) $(MY_LIB) -o $(BIN)Hash_Table.out

$(BUILDDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling \"$<\"..."
	@$(CC) $(CFLAGS) -I$(SHA_LIB_PATH) -I$(MY_LIB_PATH) $(DBG) $(OPT) -c $< -o $@

include $(DEPS)

$(BUILDDIR)%.d: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Collecting dependencies for \"$<\"..."
	@$(CC) -E $(CFLAGS) -I$(SHA_LIB_PATH) -I$(MY_LIB_PATH) $< -MM -MT $(@:.d=.o) > $@	

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
