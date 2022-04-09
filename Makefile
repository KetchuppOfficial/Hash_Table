CC = gcc
CFLAGS = -c -Wall -Werror

SHA_LIB = ../../C/SHA_256/sha_256.a
MY_LIB  = ../../C/My_Lib/My_Lib.a

all: Hash_Table

Hash_Table: main.o Hash_Table.o
	$(CC) main.o Hash_Table.o $(SHA_LIB) $(MY_LIB) -o Hash_Table.out
	rm main.o
	rm Hash_Table.o

main.o:
	$(CC) $(CFLAGS) main.c -o main.o

Hash_Table.o:
	$(CC) $(CFLAGS) Hash_Table.c -o Hash_Table.o

run:
	./Hash_Table.out

clean:
	rm Hash_Table.out
	rm log_file.log