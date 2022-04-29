#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 0

#include "../../../../SHA_256/sha_256.h"

#include "../../../../My_Lib/My_Lib.h"

#if DEBUG == 0
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
    DED_HASH_ASM,
    SHA_256
};

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

struct Hash_Table *HT_Ctor (enum Hash_Func function, uint64_t ht_size);
int HT_Search              (const struct Hash_Table *ht_ptr, const char *const data);
int HT_Delete              (struct Hash_Table *ht_ptr, const char *const data);
#endif
