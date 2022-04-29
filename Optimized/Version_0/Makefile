CC = gcc
CFLAGS = -c -g -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

SHA_LIB = ../../../../SHA_256/sha_256.a
MY_LIB  = ../../../../My_Lib/My_Lib.a

OBJECTS = main.o Hash_Table.o Fill_Hash_Table.o

OPT = 

all: Hash_Table

Hash_Table: $(OBJECTS)
	$(CC) $(OBJECTS) $(SHA_LIB) $(MY_LIB) -o Hash_Table.out
	rm $(OBJECTS)

main.o:
	$(CC) $(CFLAGS) main.c $(OPT) -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) Hash_Table_V0.c $(OPT) -o Hash_Table.o

Fill_Hash_Table.o:
	$(CC) $(CFLAGS) Fill_Hash_Table_V0.c $(OPT) -o Fill_Hash_Table.o

run:
	./Hash_Table.out

clean:
	rm Hash_Table.out
	rm -f log_file.log

profile:
	valgrind --tool=callgrind ./Hash_Table.out
	kcachegrind
	rm callgrind.out.*
