#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#include "sha_256.h"
#include "My_Lib.h"

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
    CRC_32,
    SHA_256
};

struct Hash_Table
{
    struct Node **array;
    uint32_t (* hash_func)(const char *);
    uint32_t size;
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

struct Hash_Table *HT_Ctor (enum Hash_Func function, const uint32_t ht_size);
int                HT_Dtor (struct Hash_Table *ht_ptr);

char *HT_Insert (struct Hash_Table *const ht_ptr, const char *const data);
int HT_Delete   (struct Hash_Table *const ht_ptr, const char *const data);
int HT_Search   (const struct Hash_Table *const ht_ptr, const char *const data);

int HT_Dump (const struct Hash_Table *ht_ptr);

char **HT_Fill (struct Hash_Table *ht_ptr, const char *file_name, int *n_words);
int HT_Show_Collisons (const struct Hash_Table *ht_ptr);

#endif
