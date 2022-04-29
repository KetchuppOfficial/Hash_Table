CC = gcc
CFLAGS = -c -g -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

SHA_LIB = ../../../SHA_256/sha_256.a
MY_LIB  = ../../../My_Lib/My_Lib.a

DUMP = ../Dump/
RSRCH = ../Hash_Research/

OPT = 

all: Hash_Table

Hash_Table: main.o Hash_Table.o Hash_Table_Dump.o Hash_Table_Reasearch.o
	$(CC) main.o Hash_Table.o $(DUMP)Hash_Table_Dump.o $(RSRCH)Hash_Research.o $(SHA_LIB) $(MY_LIB) -o Hash_Table.out
	rm main.o
	rm Hash_Table.o
	rm $(DUMP)Hash_Table_Dump.o
	rm $(RSRCH)Hash_Research.o

main.o:
	$(CC) $(CFLAGS) main.c $(OPT) -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) Hash_Table.c $(OPT) -o Hash_Table.o

Hash_Table_Dump.o:
	$(CC) $(CFLAGS) $(DUMP)Hash_Table_Dump.c -o $(DUMP)Hash_Table_Dump.o

Hash_Table_Reasearch.o:
	$(CC) $(CFLAGS) $(RSRCH)Hash_Research.c $(OPT) -o $(RSRCH)Hash_Research.o

run:
	./Hash_Table.out

clean:
	rm Hash_Table.out
	rm -f log_file.log

profile:
	valgrind --tool=callgrind ./Hash_Table.out
	kcachegrind
	rm callgrind.out.*
