# Hash table, its optimization and hash functions quality research

1. [General information](#general-information)
2. [Building](#building)
3. [Dependencies](#dependencies)
4. [Hash functions research](#hash-functions-research)
    1. [Cringe_1](#cringe1)
    2. [ASCII_Hash](#asciihash)
    3. [Len_Hash](#lenhash)
    4. [Checksum](#checksum)
    5. [Ded_Hash](#dedhash)
    6. [CRC_32]()
    6. [SHA_256](#sha256)
    7. [Conclusion](#conclusion)
5. [Hash table optimizaton](#hash-table-optimization)
    1. [Version_0](#version-0)
    2. [Version_1](#version-1)
    3. [Version_2](#version-2)
    4. [Version_3](#version-3)
    5. [Version_4](#version-4)
    6. [Conclusion](#conclusion-1)

# General information

This project is a C implementation of *hash table* - a well known data structure. My hash table supports 7 hash functions, which quality were carefully studied and are presented below. A unique feature of this project is its second part - optimization of hash table with the help of knowledge in processor architecture.

# Building

Using my hash table is quite simple.

**Step 1:** Clone this repository
```bash
$ git clone git@github.com:KetchuppOfficial/Hash_Table.git
```

**Step 2:** Build the project
```bash
ketchupp@ketchupp-HVY-WXX9:~/Programming/Semester_2/Hash_Table/Not_Optimized$ make
Collecting dependencies for "src/Hash_Research.c"...
Collecting dependencies for "src/Hash_Table_Dump.c"...
Collecting dependencies for "src/Hash_Table.c"...
Collecting dependencies for "src/main.c"...
Compiling "src/main.c"...
Compiling "src/Hash_Table.c"...
Compiling "src/Hash_Table_Dump.c"...
Compiling "src/Hash_Research.c"...
Linking project...
```

If you compile **Not_Opimized** version of the hash table, you can use some options:
1) You can make a .png image of the hash table using:
```bash
make OPT=-DDUMP
```
2) You can turn on all **MY_ASSERT** macros from **My_Lib** and showing the number of word in the hash table (kind of debug mode):
```bash
make OPT=-DDEBUG
```
3) Previous options can be used simultaneously:
```bash
make OPT=-DDEBUG\ -DDUMP    # don't forget backslash!
```

All version of the hash table (Not_Optimized, Version_O, ..., Version_4) are built with debug information (-g) and no optimization flags.

**Step 3:** Running
```bash
make run
```

If you wand to delete all object and dependencies files from `./build/`, run:
```bash
make clean
```

# Dependencies

This project consists of both new code and two static libraries that were also implemented by me some time ago.

## My_Lib

The 1st library is [My_Lib](https://github.com/KetchuppOfficial/My_Lib) that is used for work with files and debugging with the help of log files.

## SHA_256

The 2nd library is [SHA_256](https://github.com/KetchuppOfficial/SHA_256) that is an implementation of a well known hash function SHA-256 based on information from [Wikipedia](https://en.wikipedia.org/wiki/SHA-2).

## Paths to libraries

Set your own path(s) to folder(s) with the libraries in [Makefile](/Not_Optimized/Makefile):
```Makefile
CC     = gcc
CFLAGS = -Wall -Werror -Wshadow -Wfloat-equal -Wswitch-default

DBG = -g

SHA_LIB_PATH = /home/ketchupp/Programming/SHA_256/      # <---- here
MY_LIB_PATH  = /home/ketchupp/Programming/My_Lib/       # <---- and here

# don't forget about backslash in the end of each path!
```

# Hash functions research

**Experiment conditions.** Hash table was filled with words of *"The Lord of the Rings"*. If a word has already been added, it won't be added again. Consequently each word differs from others. It means that this experiment is a way to estimate the quality of a hash function.

My program calculates the number of collisions in every bucket and prints it into a .txt file. This data is visualized with the help of **python**. You can see information about hash functions, that were used, below.

The size of the hash table was chosen to be 2000 so that the load factor is approximately 8,24.

## Cringe_1

Returns 1 regardless of input data.
```C
static inline uint32_t Cringe_1 (const char *data)
{
    return 1;
}
```

![Cringe_1](Not_Optimized/Hash_Research/Cringe-1.png)

This function is obviously the worst hash function ever because only one list is filled with elements.

## ASCII_Hash

Returns ASCII-code of the first letter of a string.
```C
static inline uint32_t ASCII_Hash (const char *data)
{
    return (uint32_t)data[0];
}
```

![ASCII_HASH](Not_Optimized/Hash_Research/ASCII-Hash.png)

Although **ASCII_Hash** is better than **Cringe_1**, it is not of a high quality because there is a great number of empty lists in the hash table.

## Len_Hash

Returns the length of a string.
```C
static inline uint32_t Len_Hash (const char *data)
{
    return strlen (data);
}
```

![Len_Hash](Not_Optimized/Hash_Research/Len-Hash.png)

**Len_Hash** is even worse than **ASCII_HASH**: the number of empty lists increased.

## Checksum

Returns sum of ASCII-codes of letters of a string.
```C
static inline uint32_t Checksum (const char *data)
{
    uint32_t checksum = 0U;
    
    for (int i = 0; data[i] != '\0'; i++)
        checksum += data[i];

    return checksum;
}
```

![Checksum](Not_Optimized/Hash_Research/Checksum.png)

**Checksum** is definitely better than previous hash functions, but it's not good enough too. Distribution of data by lists is not uniform.

## Ded_Hash

This function is usually called **ror hash**, but in my work I named after my teacher[Ded](https://github.com/ded32), who told me about this function.
```C
static inline uint32_t ror_32 (uint32_t num, uint32_t shift)
{
    return (num >> shift) | (num << (__CHAR_BIT__ * sizeof (uint32_t) - shift));
}

static inline uint32_t Ded_Hash (const char *data)
{
    uint32_t hash = data[0];

    for (int i = 0; data[i] != '\0'; i++)
        hash = ror_32 (hash, 1) ^ data[i];

    return hash;
}
```

![Ded_Hash](Not_Optimized/Hash_Research/Ded-Hash.png)

## CRC-32

An implementation of **CRC_32** can be seen [here](Not_Optimized/src/Hash_Table.c).

![CRC_32](Not_Optimized/Hash_Research/CRC-32.png)

**CRC_32** have shown the best result among previous hash functions. The maximum number of collisions is of the same order as the load factor.

## SHA-256

An implementation of this function can be seen on the GitHub page of my SHA-256.

![SHA_256](Not_Optimized/Hash_Research/SHA-256.png)

There is no qualitative difference between **SHA_256** and **CRC_32**.

## Conclusion

**SHA-256** and **CRC-32** have shown the best result. As far as the algorithm of **SHA-256** is kind of difficult, we will use **CRC-32** at the second part of the work.

# Hash table optimization

The hash table is known for quick search of data. Thus, I implemented a test that loads **HT_Search** function to the maximum. Test function searches every word in the hash table 5000 times.

I used **callgrind** to get profiling data and **kcachegrind** to visualize it. There are some references to "clock signals" below. It means processor clock signals I got information about from the lower line of **kcachegrind** window. The performance of the hash table was also measured by the tool **time** that was ran by a bash script [measure.sh](/Optimized/measure.sh).

## Version 0

There are no optimizations in this version. Nevertheless, it differs from [Not_Optimized](Not_Optimized) version of the hash table in some ways. All differences are: 

1) All hash functions except **CRC-32** were removed;
2) Dumping hash table and making bar charts in not supported;
3) Stress test added.

As we see in the picture below, execution of *crc_32_* takes the longest time.

![profiling_data_0](/Optimized/Version_0/pictures/profiling_data.png)

It takes 40 081 087 077 clock ticks to execute the program.

Execution time:

|                       |   real   |   user   |
|-----------------------|----------|----------|
|    average time, s    |   10,4   |   10,4   |
| standard deviation, s |    0,3   |    0,3   |

As we see in the profiling data, sorted by the field *Self*, **crc_32_** function takes the longest time. That's why we will optimize this exact function. 

## Version 1

I've chosen to optimize **crc_32_** using GNU inline assembler. So I implemented crc-32 algorithm in **HT_Search**, **HT_Insert** and **HT_Delete** as in the example below:
```C
uint32_t hash = 0;
size_t len = strlen (data);

// Calculates crc-32
__asm__(
    "movl $0xFFFFFFFF, %%ebx\n\t"
    "xorq %%rcx, %%rcx\n\t"
    ".search_while:\n\t"
    "    crc32b (%1, %%rcx), %%ebx\n\t"
    "    inc %%rcx\n\t"
    "    cmp %2, %%rcx\n\t"
    "    jb .search_while\n\t"
    "xorl $0xFFFFFFFF, %%ebx\n\t"
    "movl %%ebx, %0\n\t"
    :"=r"(hash)
    :"r"(data), "r"(len)
    :"%rcx", "%ebx"
);

hash = hash % ht_ptr->size;
```

Let's see in the profiling data.

![profiling_data_1](/Optimized/Version_1/pictures/profiling_data.png)

There is no **crc_32_** at the top of the list. It means, it was worth using inline assembler. However, the *self* execution time of **HT_Search** insreased. It happened because **crc_32_** was inlined in **HT_Search**.

It takes 28 380 391 496 clock ticks to execute the program.

Execution time:

|                       |   real   |   user   |
|-----------------------|----------|----------|
|    average time, s    |   7,7    |   7,7    |
| standard deviation, s |   0,2    |   0,2    |

We've got approximatale 26% boost in time and 29% boost in the number of clock ticks. It seems to be a good result.

Profiling data shows us that we should optimize **__memcmp_avx2_movbe** next. Well, I haven't used this function in my program. It turns out **gcc** used **__memcmp_avx2_movbe** instead of **memcmp** to make memory comparison faster. I suppose I can do this job even better.

# !!! STOP HERE. THE NEXT PART IS NOT READY YET

## Version 2

I could use Intel<sup>&reg;</sup> intrinsics to accelerate *strcmp* but absence of optimization flags for the compiler ruins this idea. If we don't use -O1, intrinsics will work very slow because data will be pushed into RAM instead of registers. As I use no optimization flags, the only option is to implement a function to compare strings in the assembly language. Here is the source code (you can also look at it [here](Optimized/Version_2/Str_Cmp_SSE.s)):

```assembly
Str_Cmp_SSE:
    
        xor     r10, r10                    ; chars counter
        mov     r9, rdx                     ; saving len

        mov     rax, 16                     ; len_1 for pcmpestri          
        mov     rdx, 16                     ; len_2 for pcmpestri

        jmp     .condition

.for:

        movups  xmm1, [rdi + r10]           ; loading str_1
        movups  xmm2, [rsi + r10]           ; loading str_2

        pcmpestri xmm1, xmm2, 00001100b ; ---+
        ;                                    |
        ;                                    | LOOK HERE (find more in intel docs)
        ;                                   \|/
        ;                                    *
        ;               no effect -> 0 0 0 0 1 1 0 0 b
        ;                              \./ | \./ |  \.
        ;                          ____/   |  |   \.  128-bit sources are treated as 16 packed bytes
        ; index encodes least    /         |   \.   bytes are unsigned
        ; significant byte of res2        /      mode is equal ordered
        ;                           res2 = res1  

        test    ecx, ecx                    ; checking if string are equal
        jne     .not_equal

        add     r10, 16                     ; move to the next 16 bytes

.condition:
        cmp     r10, r9                     ; compare up to the end of str_1
        jb      .for

        xor     rax, rax                    ; return 0, if equal
        ret

.not_equal:
        mov     rax, 1                      ; return 1, if not equal
        ret   
```

The results or profiling are:

![Version_2](Optimized/Version_2/Version_2.png)
![Insert_Words_V2](Optimized/Version_2/Insert_Word.png)
![HT_Search_V2](Optimized/Version_2/HT_Search.png)

After this optimization, the program undoubtedly works faster but the boost is only 9,4% comparing to [Version_1](Optimized/Version_0) and 28,8% comparing to [Version_0](Optimized/Version_1). The result is worse than after the first optimization but still good enough to continue.

## Version_3

As they say, rewrite, so big. I was curious and looked how gcc compiles *HT_Search* function. Here is a small piece of assembly code:

```assembly
HT_Search:
.LFB15:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi
	mov	QWORD PTR -48[rbp], rsi
	mov	rax, QWORD PTR -40[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -48[rbp]
	mov	rdi, rax
	call	rdx
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -40[rbp]
	mov	rcx, QWORD PTR 16[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, 0
```

The idea is that gcc pushes parameters of the function into the stack and then works with them via RAM. No one will argue that this method is way slower than using registers. That's why I decided to rewrite the whole *HT_Search* function in the assembly language.

There is a but: since Version_3 hash table can be used only with *Ded_Hash* as a hash function. You can't choose hash function anymore. I've done this because having more than one hash function obliges to use a pointer on a function as a field of hash table structure or to implement various *HT_Search*, *HT_Insert* and *HT_Delete*: one for each hash function. The first solution decreases speed and the second is not interesting to solve. I suppose everybody can write exactly the same pieces of code replacing a specific part of them. 

Good news is that I inlined *Ded_Hash* into *HT_Search* that should make a positive impact on the performance. However, we still need *Ded_Hash* implementation in an independent file for *HT_Insert* and *HT_Delete*. You can see brand new *HT_Search* [here](Optimized/Version_3/HT_Search.s).

No let's see what have callgrind got.

![Version_3](Optimized/Version_3/Version_3.png)
![Insert_Word](Optimized/Version_3/Insert_Word.png)

The hash table accelerated by approximately 10,6% comparing to Version_2; total acceleration is about 36,3%.

Let's see on the result more carefully. *HT_Search* is still executed 5,7 times slower than *HT_Insert* and there's nothing else to optimize in *HT_Search*. Well, maybe there is but I won't compete with gcc enpowered with -O2 in the sport "Who puts variables in registers better?" as I'll lose. 

What should we do next? *Insert_Word* still takes the longest execution time of *Divide_In_Words* but *Insert_Word* is optimized as much as possible. Look at the current realization: the function is as simply as it can be (don't look at the conditional compilation).

```C
#if DEBUG == 1
static inline int Insert_Word (struct Hash_Table *ht_ptr, char *const str)
#elif DEBUG == 0
static inline void Insert_Word (struct Hash_Table *ht_ptr, char *const str)
#endif
{
    if (HT_Search (ht_ptr, str) == NOT_FOUND)
    {
        #if DEBUG == 0
        HT_Insert (ht_ptr, str);
        #elif DEBUG == 1
        int ret_val = HT_Insert (ht_ptr, str);
        MY_ASSERT (ret_val != ERROR, "HT_Insert ()", FUNC_ERROR, ERROR);
        return 1;
        #endif
    }

    #if DEBUG == 1
    return 0;
    #endif
}
```

Of course, I can try to speed *HT_Insert* up but it won't result in a great increase in the performance as *HT_Insert* already works way faster than *HT_Search* (it's on the 9th position in the callgrind list). I think it's time to leave *Insert_Word* and move forward.

## Version_4

The next function that can be boosted is mysterious *__ctype_b_loc*. Actually, there is no mystery, just good old *isalpha*. Let's completely remove it by changing
```C
if ( isalpha (buffer[symb_i]) || buffer[symb_i] == '\'')
    str[letter_i++] = buffer[symb_i];
```
into
```C
if ( ('a' <= buffer[symb_i] && buffer[symb_i] <= 'z') || 
     ('A' <= buffer[symb_i] && buffer[symb_i] <= 'Z') || 
                               buffer[symb_i] == '\'')
    {
        str[letter_i++] = buffer[symb_i];
    }
```
and look what it will lead to.

Callgrind arrives with great news!

![Version_4](Optimized/Version_4/Version_4.png)

Comparing to Version_3, the boost is approximately 10,8%. Total boost is about 42,9%.

As we see in the last picture, functions on lines 2-4 are already optimized. Optimizing *HT_Insert* isn't reasonalbe (as it was mentioned). I'm obviously not good enough to make *malloc*, *calloc*, *strlen* and *memcpy* better than they already are. We can continue improving performance of *Divide_In_Words* but we won't. Firstly, this function is just a test for functions that work with hash table such as *HT_Insert* and *HT_Search*. That's why optimizing it won't optimize hash table itself. Secondly, apart from calling *HT_Search* and *HT_Insert* *Divide_In_Words* has conditional operators and a cycle. We could try to optimize them by implementing this function in the assembly language but gcc with -O2 flag would do a better job. So, Version_4 is the last one.

## Conclusion

We succeded in accelerating the work of hash table. It took 290 210 517 clock signals to execute Version_0 and only 165 580 372 to execute Version_4 that is approximately 42,9% boost. Let's compare the final result with Version_0 compiled with flags -O1, -O2 and -O3.

|                 | Clock signals |
|-----------------|---------------|
| My optimization |  165 580 372  |
|       -O1       |  134 331 275  |
|       -O2       |  135 796 371  |
|       -O3       |  135 617 014  |

The result is that my optimization is relatively close to the result of gcc with one of optimization flags -O1, -O2 or -O3. Let's calculate the most important coefficient that is widely known Ded_Coefficient.

Ded_Coefficient = (acceleration value / number of assembly lines) * 1000

We will count only instruction and won't count comments, labels or names of functions. Taking this into consideration, we find: Ded_Hash.s - 10 lines, HT_Search.s - 48 lines, Str_Cmp_SSE.s - 17 lines. All in all:

Ded_Coefficient = (43,2 / 75) * 1000 = 572
