#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#include "../../../../My_Lib/My_Lib.h"

#ifndef DEBUG
#undef MY_ASSERT
#define MY_ASSERT(condition, var, err_num, error) ;
#endif

struct Node;
struct Hash_Table;

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

enum Status
{
    NOT_FOUND = -1
};

struct Hash_Table *HT_Ctor (enum Hash_Func function, const uint32_t ht_size);
int                HT_Dtor (struct Hash_Table *ht_ptr);

char *HT_Insert (struct Hash_Table *ht_ptr, const char *const data);
int   HT_Delete (struct Hash_Table *ht_ptr, const char *const data);
int   HT_Search (const struct Hash_Table *ht_ptr, const char *const data);

char **HT_Fill (struct Hash_Table *ht_ptr, const char *file_name, int *n_words);
void   HT_Test (const struct Hash_Table *ht_ptr, char **words_arr, const int n_words, const int n_tests);

#endif
