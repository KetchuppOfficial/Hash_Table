#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <immintrin.h>

#include "My_Lib.h"

#ifndef DEBUG
#undef MY_ASSERT
#define MY_ASSERT(condition, var, err_num, error) ;
#endif

#define MAX_WORD_LEN 32

struct Node;
struct Hash_Table;

enum Status
{
    NOT_FOUND = -1
};

struct Hash_Table *HT_Ctor (const uint32_t ht_size);
int                HT_Dtor (struct Hash_Table *ht_ptr);

char *HT_Insert (struct Hash_Table *ht_ptr, const char *const data);
int   HT_Delete (struct Hash_Table *ht_ptr, const char *const data);
int   HT_Search (const struct Hash_Table *ht_ptr, const char *const data);

char **HT_Fill (struct Hash_Table *ht_ptr, const char *file_name, int *n_words);
void   HT_Test (const struct Hash_Table *ht_ptr, char **words_arr, const int n_words, const int n_tests);

#endif
