CC = gcc
CFLAGS = -c -g -Wall  -Wshadow -Wfloat-equal -Wswitch-default

SHA_LIB = ../../../SHA_256/sha_256.a
MY_LIB  = ../../../My_Lib/My_Lib.a

OBJECTS = main.o Hash_Table.o Fill_Hash_Table.o HT_Insert.o HT_Search.o Str_Cmp_Align16.o

OPT = 

all: Hash_Table

Hash_Table: $(OBJECTS)
	$(CC) -no-pie $(OBJECTS) $(SHA_LIB) $(MY_LIB) -o Hash_Table.out
	rm $(OBJECTS)

# C language
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main.o:
	$(CC) $(CFLAGS) main.c            $(OPT) -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) Hash_Table.c      $(OPT) -o Hash_Table.o

Fill_Hash_Table.o:
	$(CC) $(CFLAGS) Fill_Hash_Table.c $(OPT) -o Fill_Hash_Table.o
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Assembly language
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HT_Search.o:
	nasm -f elf64 -g HT_Search.s       -o HT_Search.o

HT_Insert.o:
	nasm -f elf64 -g HT_Insert.s       -o HT_Insert.o

Str_Cmp_Align16.o:
	nasm -f elf64 -g Str_Cmp_Align16.s -o Str_Cmp_Align16.o
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

run:
	./Hash_Table.out

clean:
	rm Hash_Table.out
	rm -f log_file.log

profile:
	valgrind --tool=callgrind ./Hash_Table.out
	kcachegrind
	rm callgrind.out.*
