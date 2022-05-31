CC     = gcc
CFLAGS = -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default -mavx2

BIN      = ./bin/
SRCDIR   = ./src/
BUILDDIR = ./build/

SRC_LIST = main.c Hash_Table.c Fill_Hash_Table.c
ASM_LIST = HT_Search.s

SRC = $(addprefix $(SRCDIR),$(SRC_LIST))
ASM = $(addprefix $(SRCDIR),$(ASM_LIST))

SUBS := $(SRC)
SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(SUBS))

ASM_SUBS := $(ASM)
ASM_SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(ASM_SUBS))

C_OBJ = $(SUBS:.c=.o)
ASM_OBJ = $(ASM_SUBS:.s=.o)
DEPS = $(SUBS:.c=.d)

LIBS_LIST = My_Lib
LIBSDIR = $(addprefix ../../lib/, $(LIBS_LIST))
LIBS = $(addsuffix /*.a, $(LIBSDIR))
FIND = $(addprefix -I, $(LIBSDIR))

.PHONY: all $(LIBSDIR)

all: $(DEPS) $(C_OBJ) $(ASM_OBJ) $(LIBSDIR)
	@mkdir -p $(BIN)
	@echo "Linking project..."
	@$(CC) -no-pie $(C_OBJ) $(ASM_OBJ) $(LIBS) -o $(BIN)Hash_Table.out

$(LIBSDIR):
	@$(MAKE) -C $@ --no-print-directory -f Makefile.mak

$(BUILDDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling \"$<\"..."
	@$(CC) $(CFLAGS) $(FIND) -c -g $(OPT) $< -o $@

$(BUILDDIR)%.o: $(SRCDIR)%.s
	@mkdir -p $(dir $@)
	@echo "Assembling \"$<\"..."
	@nasm -f elf64 $< -o $@

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
	rm -rf $(C_OBJ) $(ASM_OBJ) $(DEPS)

profile: $(BIN)Hash_Table.out
	@echo "Profiling \"$<\"..."
	@valgrind --tool=callgrind $(BIN)Hash_Table.out
	@kcachegrind
	@rm -rf callgrind.out.*
