CC = gcc
CFLAGS = -g -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

SHA_LIB = ../../../SHA_256/sha_256.a
MY_LIB  = ../../../My_Lib/My_Lib.a

DUMP = ../Dump/
RSRCH = ../Hash_Research/

OPT = 

all: Hash_Table clean

Hash_Table: main.o Hash_Table.o Hash_Table_Dump.o Hash_Table_Reasearch.o
	$(CC) main.o Hash_Table.o $(DUMP)Hash_Table_Dump.o $(RSRCH)Hash_Research.o $(SHA_LIB) $(MY_LIB) -o Hash_Table.out

main.o:
	$(CC) $(CFLAGS) -c main.c $(OPT) -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) -c Hash_Table.c $(OPT) -o Hash_Table.o

Hash_Table_Dump.o:
	$(CC) $(CFLAGS) -c $(DUMP)Hash_Table_Dump.c -o $(DUMP)Hash_Table_Dump.o

Hash_Table_Reasearch.o:
	$(CC) $(CFLAGS) -c $(RSRCH)Hash_Research.c $(OPT) -o $(RSRCH)Hash_Research.o

run:
	./Hash_Table.out

clean:
	rm main.o Hash_Table.o $(DUMP)Hash_Table_Dump.o $(RSRCH)Hash_Research.o

profile:
	valgrind --tool=callgrind ./Hash_Table.out
	kcachegrind
	rm callgrind.out.*
