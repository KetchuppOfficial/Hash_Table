#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#define DUMP 1

#include "/home/ketchupp/Programming/SHA_256/sha_256.h"

#include "/home/ketchupp/Programming/My_Lib/My_Lib.h"

#ifndef DEBUG
#undef MY_ASSERT
#define MY_ASSERT(condition, var, err_num, error) ;
#endif

enum Hash_Func
{
    CRINGE_1,
    ASCII_HASH,
    LEN_HASH,
    CHECKSUM,
    DED_HASH,
    SHA_256
};

struct Hash_Table
{
    struct Node **array;
    uint64_t (* hash_func)(const char *);
    uint64_t size;
    enum Hash_Func hash_func_name;
};

struct Node
{
    char *data;
    size_t len;
    struct Node *next;
};

enum Status
{
    NOT_FOUND = -1
};

struct Hash_Table *HT_Ctor (enum Hash_Func function, const uint64_t ht_size);
int                HT_Dtor (struct Hash_Table *ht_ptr);

int HT_Insert (struct Hash_Table *ht_ptr, const char *const data);
int HT_Delete (struct Hash_Table *ht_ptr, const char *const data);
int HT_Search (const struct Hash_Table *ht_ptr, const char *const data);

int HT_Dump (const struct Hash_Table *ht_ptr);

int HT_Fill (struct Hash_Table *ht_ptr, const char *file_name);
int HT_Show_Collisons (const struct Hash_Table *ht_ptr);

#endif
