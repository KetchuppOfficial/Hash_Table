# Hash table, its optimization and hash funcions' quality research

1. [General information](#general-information)
2. [Building](#building)
3. [Dependencies](#dependencies)
4. [Hash functions research](#hash-functions-research)

# General information

This project is C implementation of *hash table* - a well known data structure. My hashtable supports 6 hash functions which quality was carefully studied and is presented below. A unique feature of this projects is its second part - optimization of hash table with the help of knowlendge in processor architecture.

# Building

Using my hash table is quite simple.

**Step 1:** Clone this repository
```
$ git clone git@github.com:KetchuppOfficial/Hash_Table.git
```

**Step 2:** Build the project
```
$ make
```
If building ended well you'll see a similar message:
```
ketchupp@LAPTOP-LS9003PM:/mnt/d/Programming/Assembler/Hash_Table$ make
gcc -c -Wall -Werror main.c -o main.o
gcc -c -Wall -Werror Hash_Table.c -o Hash_Table.o
gcc -c -Wall -Werror Dump/Hash_Table_Dump.c -o Dump/Hash_Table_Dump.o
gcc -c -Wall -Werror Hash_Research/Hash_Research.c -o Hash_Research/Hash_Research.o
gcc main.o Hash_Table.o Dump/Hash_Table_Dump.o Hash_Research/Hash_Research.o ../../C/SHA_256/sha_256.a ../../C/My_Lib/My_Lib.a -o Hash_Table.out
rm main.o
rm Hash_Table.o
rm Dump/Hash_Table_Dump.o
rm Hash_Research/Hash_Research.o
```

**Step 3:** Running
```
$ make run
```

If you wand to delete an executable file (Hash_Table.out) and a log file (log_file.log), run:
```
$ make clean
```

# Dependencies

This project consists of both new code and two static libraries that were also implemented by me some time ago.

## My_Lib

The 1st library is [My_Lib](https://github.com/KetchuppOfficial/My_Lib) that is used for work with files and debugging with the help of log files.

## SHA_256

The 2nd library is [SHA_256](https://github.com/KetchuppOfficial/SHA_256) that is an implementation of a well known hash function SHA-256 based on information from [Wikipedia](https://en.wikipedia.org/wiki/SHA-2).

## Paths to libraries

If relative paths from files of this projects to the mentioned libraries differ from the default ones, you have to change them in [Hash_Table.h](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Table.h).

```C
#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#include "../../C/SHA_256/sha_256.h"  // <------- Change here

#include "../../C/My_Lib/My_Lib.h"    // <------- And here
```

# Hash functions research

**Experiment conditions.** Hash table was filled with words of *"The Lord of the Rings"*. If a word has already been added, it won't be added again. Sequently, each word differs from others. Choosing an exact size of the hash table was based on specifics of each hash function.

As it was mentioned earlier, my hash table can use one of 6 hash function. The following is pieces of information about each on of them and graphs that show a number of worlds in every cell of the hash table. As far as two words in any pair differ from each other, this experiment is a way to estimate the number of collisions of all 6 hash functions.

## Cringe_1

Returns 1 regardless of input data.
```C
static uint64_t Cringe_1 (const char *data)
{
    return 1;
}
```

**Hash table size:** 20 cells.

As far as the output of *Cringe_1 ()* is known from the very beginnig, the size was chosen resonably small.

![Cringe_1](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Research/CRINGE_1.png)

## ASCII_Hash

Returns ASCII-code of the first letter of a string.
```C
static uint64_t ASCII_Hash (const char *data)
{
    return (uint64_t)data[0];
}
```
**Hash table size:** 128 cells. 

Since ASCII-codes belong to [0; 255] and John Ronald Reuel Tolkien could use only first 128 of them, the size was chosen appropriate.

![ASCII_HASH](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Research/ASCII_HASH.png)

## Len_Hash

Returns the length of a string.
```C
static uint64_t Len_Hash (const char *data)
{
    return strlen (data);
}
```
**Hash table size:** 50 cells. 

As the ordinary length of words in fiction is obviously less than 50, the size was chosed a little overstated.

![Len_Hash](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Research/LEN_HASH.png)

## Checksum

Returns sum of ASCII-codes of letters of a string.
```C
static uint64_t Checksum (const char *data)
{
    uint64_t checksum = 0U;
    
    for (int i = 0; data[i] != '\0'; i++)
        checksum += data[i];

    return checksum;
}
```
**Hash table size:** 4096 cells. 

Three previous function was a kind of joke and shouldn't be treated seriously. Since *Checksum ()* hash function are *real*, if it can be said this way. Here and further the size of the hash table is 4096 cells. If an ideal hash function is used, there will be approximatelly 14 words in each cell.

![Checksum](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Research/CHECKSUM.png)

## Ded_Hash

I was informed about this function by my teacher [Ded](https://github.com/ded32).
```C
static inline uint64_t rotr (uint64_t num, uint64_t shift)
{
    return (num >> shift) | (num << (64 - shift));
}

static uint64_t Ded_Hash (const char *data)
{
    uint64_t hash = data[0];

    for (int i = 0; data[i] != '\0'; i++)
        hash = rotr (hash, 1) ^ data[i];

    return hash;
}
```

**Hash table size:** 2048 cells

![Ded_Hash_2048](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Research/DED_HASH.png)

**Hash table size:** 2047 cells

![Ded_Hash_2047]()

You can see that **Ded_Hash** is not good to use if the size of a hash table is multiple of 2.

## SHA-256

Implementation of this function can be seen on the GitHub page of my SHA-256.

**Hash table size:** 4096 cells

![SHA_256](https://github.com/KetchuppOfficial/Hash_Table/blob/master/Hash_Research/SHA_256.png)
