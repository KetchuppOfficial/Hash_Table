CC = gcc
CFLAGS = -c -g -Wall  -Wshadow -Wfloat-equal -Wswitch-default

SHA_LIB = ../../../../SHA_256/sha_256.a
MY_LIB  = ../../../../My_Lib/My_Lib.a

OBJECTS = main.o Hash_Table.o Fill_Hash_Table.o Ded_Hash.o

OPT = 

all: Hash_Table

Hash_Table: $(OBJECTS)
	$(CC) -no-pie $(OBJECTS) $(SHA_LIB) $(MY_LIB) -o Hash_Table.out
	rm $(OBJECTS)

# C language
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main.o:
	$(CC) $(CFLAGS) main.c               $(OPT) -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) Hash_Table_V1.c      $(OPT) -o Hash_Table.o

Fill_Hash_Table.o:
	$(CC) $(CFLAGS) Fill_Hash_Table_V1.c $(OPT) -o Fill_Hash_Table.o
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Assembly language
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Ded_Hash.o:
	nasm -f elf64 -g Ded_Hash.s -o Ded_Hash.o
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
