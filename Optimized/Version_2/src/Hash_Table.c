#include "../Hash_Table.h"

struct Node
{
    char *data;
    struct Node *next;
};

struct Hash_Table
{
    struct Node **array;
    uint32_t size;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                  CONSTRUCTORS AND DESTRUCTORS                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Hash_Table *HT_Ctor (const uint32_t ht_size)
{
    MY_ASSERT (ht_size > 0, "const uint32_t ht_size", POS_VAL, NULL);
    
    struct Hash_Table *ht_ptr = (struct Hash_Table *)calloc (1, sizeof (struct Hash_Table));
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NE_MEM, NULL);

    ht_ptr->array = (struct Node **)calloc (ht_size, sizeof (struct Node *));
    MY_ASSERT (ht_ptr->array, "ht_ptr->array", NE_MEM, NULL);

    ht_ptr->size = ht_size;

    return ht_ptr;
}

static int List_Dtor (struct Node *node_ptr)
{
    MY_ASSERT (node_ptr, "struct Node *node_ptr", NULL_PTR, ERROR);

    struct Node *current = node_ptr;
    struct Node *prev    = NULL;
    
    while (current->next != NULL)
    {
        prev = current;
        current = current->next;

        free (prev->data);
        free (prev);
    }

    free (current->data);
    free (current);

    return NO_ERRORS;
}

int HT_Dtor (struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    for (uint32_t i = 0; i < ht_ptr->size; i++)
    {
        if (ht_ptr->array[i] != NULL)
            List_Dtor (ht_ptr->array[i]);
    }

    free (ht_ptr->array);
    free (ht_ptr);

    return NO_ERRORS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                            INSERTION                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static struct Node *Add_Node (char *data)
{
    MY_ASSERT (data, "const char *data", NULL_PTR, NULL);

    struct Node *new_node = (struct Node *)calloc (1, sizeof (struct Node));
    MY_ASSERT (new_node, "struct Node *new_node", NE_MEM, NULL);

    new_node->next = NULL;
    new_node->data = data;

    return new_node;
}

char *HT_Insert (struct Hash_Table *ht_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, NULL);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, NULL);

    char *str = (char *)calloc (MAX_WORD_LEN + 1, sizeof (char));
    MY_ASSERT (str, "char *str", NE_MEM, NULL);

    memcpy (str, data, strlen (data));

    uint32_t hash = 0;

    // Calculates crc-32
    __asm__(
        "or $0xFFFFFFFFFFFFFFFF, %%rax\n\t"
        "crc32q (%1),     %%rax\n\t"
        "crc32q 0x08(%1), %%rax\n\t"
        "crc32q 0x10(%1), %%rax\n\t"
        "crc32q 0x18(%1), %%rax\n\t"
        "not %%rax\n\t"
        "movl %%eax, %0\n\t"
        :"=r"(hash)
        :"r"(str)
        :"%rax"
    );

    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
    {
        ht_ptr->array[hash] = Add_Node (str);
        MY_ASSERT (ht_ptr->array[hash], "Add_Node ()", FUNC_ERROR, NULL);
    }
    else
    {      
        struct Node **list_arr = ht_ptr->array;
        
        struct Node *old_first_node = list_arr[hash];
        list_arr[hash] = Add_Node (str);
        MY_ASSERT (list_arr[hash], "Add_Node ()", FUNC_ERROR, NULL);
        list_arr[hash]->next = old_first_node;
    }

    return ht_ptr->array[hash]->data;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                     SEARCHING AND DELETING                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#define RIGHT_MASK 0xFFFFFFFF
static inline int Fast_Cmp (const char *str_1, const char *str_2)
{      
    __m256i STR_1 = _mm256_loadu_si256 ((__m256i *)(str_1));
    __m256i STR_2 = _mm256_loadu_si256 ((__m256i *)(str_2));

    __m256i mask_avx = _mm256_cmpeq_epi8 (STR_1, STR_2);
    if (_mm256_movemask_epi8 (mask_avx) == RIGHT_MASK)
        return 0;

    return 1;
}
#undef RIGHT_MASK

static int List_Search (struct Node *const list_ptr, const char *const data)
{
    MY_ASSERT (list_ptr, "struct Node *const list_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,     "const char *const data",      NULL_PTR, ERROR);
    
    struct Node *current = list_ptr;
    struct Node *next    = list_ptr->next;

    int node_i = 0;
    for ( ; next != NULL; node_i++)
    {
        if (Fast_Cmp (current->data, data) == 0)
            return node_i;
        else
        {
            current = next;
            next = current->next;
        }
    }

    return (Fast_Cmp (current->data, data) == 0) ? node_i : NOT_FOUND;
}

int HT_Search (const struct Hash_Table *const ht_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *const data",          NULL_PTR, ERROR);
    
    uint32_t hash = 0;

    // Calculates crc-32
    __asm__(
        "or $0xFFFFFFFFFFFFFFFF, %%rax\n\t"
        "crc32q (%1),     %%rax\n\t"
        "crc32q 0x08(%1), %%rax\n\t"
        "crc32q 0x10(%1), %%rax\n\t"
        "crc32q 0x18(%1), %%rax\n\t"
        "not %%rax\n\t"
        "movl %%eax, %0\n\t"
        :"=r"(hash)
        :"r"(data)
        :"%rax"
    );

    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
        return NOT_FOUND;
    else
        return List_Search (ht_ptr->array[hash], data);
}

static inline void Delete_Beg_ (struct Hash_Table *const ht_ptr, const uint32_t hash, struct Node *const current)
{
    ht_ptr->array[hash] = (current->next == NULL) ? NULL : current->next;

    free (current->data);
    free (current);
}

static inline void Delete_Mid_ (struct Node *const prev, struct Node *const current, struct Node *const next)
{
    prev->next = next;

    free (current->data);
    free (current);
}

static inline void Delete_End_ (struct Node *const prev, struct Node *const current)
{
    prev->next = NULL;
    
    free (current->data);
    free (current);
}

static int List_Delete (struct Hash_Table *const ht_ptr, const uint32_t hash, struct Node *const list_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr,   "struct Hash_Table *ht_ptr",   NULL_PTR, ERROR);
    MY_ASSERT (list_ptr, "struct Node *const list_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,     "const char *const data",      NULL_PTR, ERROR);
    
    struct Node *prev    = NULL;
    struct Node *current = list_ptr;
    struct Node *next    = current->next;

    int node_i = 0;
    for ( ; next != NULL; node_i++)
    {
        if (Fast_Cmp (current->data, data) == 0)
        {               
            if (node_i == 0)
                Delete_Beg_ (ht_ptr, hash, current);
            else
                Delete_Mid_ (prev, current, next);

            return NO_ERRORS;
        }
        else
        {
            prev    = current;
            current = next;
            next    = current->next;
        }
    }

    if (Fast_Cmp (current->data, data) == 0)
    {
        if (node_i == 0)
            Delete_Beg_ (ht_ptr, hash, current);
        else
            Delete_End_ (prev, current);

        return NO_ERRORS;
    }
    else
        return NOT_FOUND;
}

int HT_Delete (struct Hash_Table *const ht_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *const data",    NULL_PTR, ERROR);

    uint32_t hash = 0;

    // Calculates crc-32
    __asm__(
        "or $0xFFFFFFFFFFFFFFFF, %%rax\n\t"
        "crc32q (%1),     %%rax\n\t"
        "crc32q 0x08(%1), %%rax\n\t"
        "crc32q 0x10(%1), %%rax\n\t"
        "crc32q 0x18(%1), %%rax\n\t"
        "not %%rax\n\t"
        "movl %%eax, %0\n\t"
        :"=r"(hash)
        :"r"(data)
        :"%rax"
    );

    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
        return NOT_FOUND;
    else
        return List_Delete (ht_ptr, hash, ht_ptr->array[hash], data);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
