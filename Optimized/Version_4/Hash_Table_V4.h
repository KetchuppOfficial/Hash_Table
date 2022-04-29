#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define DEBUG 0

#include "../../../../My_Lib/My_Lib.h"

#if DEBUG == 0
#undef MY_ASSERT
#define MY_ASSERT(condition, var, err_num, error) ;
#endif

struct Node;
struct Hash_Table;

enum Status
{
    NOT_FOUND = -1
};

#if DEBUG == 1

int HT_Dtor   (struct Hash_Table *ht_ptr);

int HT_Insert (struct Hash_Table *ht_ptr, const char *const data);

int HT_Fill   (struct Hash_Table *ht_ptr, const char *file_name);

#elif DEBUG == 0

void HT_Dtor   (struct Hash_Table *ht_ptr);

void HT_Insert (struct Hash_Table *ht_ptr, const char *const data);

void HT_Fill   (struct Hash_Table *ht_ptr, const char *file_name);

#endif

struct Hash_Table *HT_Ctor (const uint64_t ht_size);
int HT_Search              (const struct Hash_Table *ht_ptr, const char *const data);
int HT_Delete              (struct Hash_Table *ht_ptr, const char *const data);
#endif
