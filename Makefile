CC = gcc
CFLAGS = -c -Wall -Werror

SHA_LIB = ../../C/SHA_256/sha_256.a
MY_LIB  = ../../C/My_Lib/My_Lib.a

DUMP = Dump/
RSRCH = Hash_Research/

all: Hash_Table

Hash_Table: main.o Hash_Table.o Hash_Table_Dump.o Hash_Table_Reasearch.o
	$(CC) main.o Hash_Table.o $(DUMP)Hash_Table_Dump.o $(RSRCH)Hash_Research.o $(SHA_LIB) $(MY_LIB) -o Hash_Table.out
	rm main.o
	rm Hash_Table.o
	rm $(DUMP)Hash_Table_Dump.o
	rm $(RSRCH)Hash_Research.o

main.o:
	$(CC) $(CFLAGS) main.c -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) Hash_Table.c -o Hash_Table.o

Hash_Table_Dump.o:
	$(CC) $(CFLAGS) $(DUMP)Hash_Table_Dump.c -o $(DUMP)Hash_Table_Dump.o

Hash_Table_Reasearch.o:
	$(CC) $(CFLAGS) $(RSRCH)Hash_Research.c -o $(RSRCH)Hash_Research.o

run:
	./Hash_Table.out

clean:
	rm Hash_Table.out
	rm log_file.log

callgrind:
	valgrind --tool=callgrind ./Hash_Table.out
	callgrind_annotate callgrind.out.* --tree=both > Optimization/callgrind.txt
	rm callgrind.out.*
