#include "Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                         HASH FUNCTIONS                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static uint64_t Cringe_1 (const char *data)
{
    return 1;
}

static uint64_t ASCII_Hash (const char *data)
{
    return (uint64_t)data[0];
}

static uint64_t Len_Hash (const char *data)
{
    return strlen (data);
}

static uint64_t Checksum (const char *data)
{
    uint64_t checksum = 0U;
    
    for (int i = 0; data[i] != '\0'; i++)
        checksum += data[i];

    return checksum;
}

static inline uint64_t ror (uint64_t num, uint64_t shift)
{
    return (num >> shift) | (num << (64 - shift));
}

static uint64_t Ded_Hash (const char *data)
{
    uint64_t hash = data[0];

    for (int i = 0; data[i] != '\0'; i++)
        hash = ror (hash, 1) ^ data[i];

    return hash;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                  CONSTRUCTORS AND DESTRUCTORS                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Hash_Table *HT_Ctor (enum Hash_Func function, uint64_t ht_size)
{
    struct Hash_Table *ht_ptr = (struct Hash_Table *)calloc (1, sizeof (struct Hash_Table));
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NE_MEM, NULL);

    ht_ptr->array = (struct Node **)calloc (ht_size, sizeof (struct Node *));
    MY_ASSERT (ht_ptr->array, "ht_ptr->array", NE_MEM, NULL);

    ht_ptr->size = ht_size;

    switch (function)
    {
        case CRINGE_1:
            ht_ptr->hash_func      = Cringe_1;
            ht_ptr->hash_func_name = CRINGE_1;
            break;

        case ASCII_HASH:
            ht_ptr->hash_func      = ASCII_Hash;
            ht_ptr->hash_func_name = ASCII_HASH;
            break;

        case LEN_HASH:
            ht_ptr->hash_func      = Len_Hash;
            ht_ptr->hash_func_name = LEN_HASH;
            break;

        case CHECKSUM:
            ht_ptr->hash_func      = Checksum;
            ht_ptr->hash_func_name = CHECKSUM;
            break;
        
        case DED_HASH:
            ht_ptr->hash_func      = Ded_Hash;
            ht_ptr->hash_func_name = DED_HASH;
            break;

        case SHA_256:
            ht_ptr->hash_func      = sha_256_64;
            ht_ptr->hash_func_name = SHA_256;
            break;
    
        default:
            MY_ASSERT (false, "enum Hash_Func function", UNEXP_VAL, NULL);
    }

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
    
    for (uint64_t i = 0; i < ht_ptr->size; i++)
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

static struct Node *Add_Node (const char *data)
{
    MY_ASSERT (data, "const char *data", NULL_PTR, NULL);

    struct Node *new_node = (struct Node *)calloc (1, sizeof (struct Node));
    MY_ASSERT (new_node, "struct Node *new_node", NE_MEM, NULL);

    size_t len = strlen (data) + 1;
    char *str = (char *)calloc (len, sizeof (char));
    MY_ASSERT (str, "char *str", NE_MEM, NULL);

    memcpy (str, data, len);

    new_node->next = NULL;
    new_node->data = str;
    new_node->len  = len - 1;

    return new_node;
}

int HT_Insert (struct Hash_Table *ht_ptr, const char *data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, ERROR);

    uint64_t hash = (* ht_ptr->hash_func)(data);
    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
    {
        ht_ptr->array[hash] = Add_Node (data);

        #if DEBUG == 1
        if (ht_ptr->array[hash] == NULL)
            MY_ASSERT (false, "Add_Node ()", FUNC_ERROR, ERROR);
        #endif
    }
    else
    {
        struct Node *current = ht_ptr->array[hash];
        struct Node *next    = current->next;

        while (next != NULL)
        {
            current = next;
            next = current->next;
        }

        current->next = Add_Node (data);
        #if DEBUG == 1
        if (current->next == NULL)
            MY_ASSERT (false, "Add_Node ()", FUNC_ERROR, ERROR);
        #endif
    }

    return NO_ERRORS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                     SEARCHING AND DELETING                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int HT_Search (const struct Hash_Table *ht_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *data",                NULL_PTR, ERROR);
    
    uint64_t hash = (* ht_ptr->hash_func)(data);
    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
        return NOT_FOUND;
    else
    {
        struct Node *current = ht_ptr->array[hash];
        struct Node *next    = current->next;

        int node_i = 0;
        for ( ; next != NULL; node_i++)
        {           
            if (memcmp (current->data, data, current->len + 1) == 0)
                return node_i;
            else
            {
                current = next;
                next = current->next;
            }
        }

        return (memcmp (current->data, data, current->len + 1) == 0) ? node_i : NOT_FOUND;
    }
}

static inline void Delete_Beg_ (struct Hash_Table *ht_ptr, const uint64_t hash, struct Node *current)
{
    ht_ptr->array[hash] = (current->next == NULL) ? NULL : current->next;

    free (current->data);
    free (current);
}

static inline void Delete_Mid_ (struct Node *prev, struct Node *current, struct Node *next)
{
    prev->next = next;

    free (current->data);
    free (current);
}

static inline void Delete_End_ (struct Node *prev, struct Node *current)
{
    prev->next = NULL;
    
    free (current->data);
    free (current);
}

int HT_Delete (struct Hash_Table *ht_ptr, const char *data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, ERROR);

    uint64_t hash = (* ht_ptr->hash_func)(data);
    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
        return NOT_FOUND;
    else
    {
        struct Node *prev    = NULL;
        struct Node *current = ht_ptr->array[hash];
        struct Node *next    = current->next;

        int node_i = 0;
        for ( ; next != NULL; node_i++)
        {
            if (memcmp (current->data, data, current->len + 1) == 0)
            {               
                if (node_i == 0)
                    Delete_Beg_ (ht_ptr, hash, current);
                else
                    Delete_Mid_ (prev, current, next);

                return NO_ERRORS;
            }
            else
            {
                prev = current;
                current = next;
                next = current->next;
            }
        }

        if (memcmp (current->data, data, current->len + 1) == 0)
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
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
