#include "Hash_Table_V4.h"

struct Node
{
    char *str;
    size_t len;
    struct Node *next;
};

struct Hash_Table
{
    struct Node **array;
    uint64_t size;
};

uint64_t Ded_Hash (const char *data);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                  CONSTRUCTORS AND DESTRUCTORS                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Hash_Table *HT_Ctor (const uint64_t ht_size)
{
    MY_ASSERT (ht_size > 0, "const uint64_t ht_size", POS_VAL, NULL);
    
    struct Hash_Table *ht_ptr = (struct Hash_Table *)calloc (1, sizeof (struct Hash_Table));
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NE_MEM, NULL);

    ht_ptr->array = (struct Node **)calloc (ht_size, sizeof (struct Node *));
    MY_ASSERT (ht_ptr->array, "ht_ptr->array", NE_MEM, NULL);

    ht_ptr->size = ht_size;
    
    return ht_ptr;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if DEBUG == 1
static int List_Dtor (struct Node *node_ptr)
#elif DEBUG == 0
static void List_Dtor (struct Node *node_ptr)
#endif
{
    MY_ASSERT (node_ptr, "struct Node *node_ptr", NULL_PTR, ERROR);

    struct Node *current = node_ptr;
    struct Node *prev    = NULL;
    
    while (current->next != NULL)
    {
        prev = current;
        current = current->next;

        free (prev->str);
        free (prev);
    }

    free (current->str);
    free (current);

    #if DEBUG == 1
    return NO_ERRORS;
    #endif
}

#if DEBUG == 1
int HT_Dtor (struct Hash_Table *ht_ptr)
#elif DEBUG == 0
void HT_Dtor (struct Hash_Table *ht_ptr)
#endif
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    uint64_t ht_size = ht_ptr->size;
    
    for (uint64_t i = 0U; i < ht_size; i++)
    {
        if (ht_ptr->array[i] != NULL)
            List_Dtor (ht_ptr->array[i]);
    }

    free (ht_ptr->array);
    free (ht_ptr);

    #if DEBUG == 1
    return NO_ERRORS;
    #endif
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                            INSERTION                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Node *Add_Node (const char *data)
{
    MY_ASSERT (data, "const char *data", NULL_PTR, NULL);

    struct Node *new_node = (struct Node *)calloc (1, sizeof (struct Node));
    MY_ASSERT (new_node, "struct Node *new_node", NE_MEM, NULL);

    size_t len = strlen (data);
    size_t new_len = ((len >> 4) + 1) << 4;

    char *str = (char *)calloc (new_len, sizeof (char));
    MY_ASSERT (str, "char *str", NE_MEM, NULL);

    memcpy (str, data, len);

    new_node->next = NULL;
    new_node->str  = str;
    new_node->len  = new_len;

    return new_node;
}

#if 1

#if DEBUG == 1
int HT_Insert (struct Hash_Table *ht_ptr, const char *data)
#elif DEBUG == 0
void HT_Insert (struct Hash_Table *ht_ptr, const char *data)
#endif
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, ERROR);

    uint64_t hash = Ded_Hash(data);
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
            next    = current->next;
        }

        current->next = Add_Node (data);
        #if DEBUG == 1
        if (current->next == NULL)
            MY_ASSERT (false, "Add_Node ()", FUNC_ERROR, ERROR);
        #endif
    }

    #if DEBUG == 1
    return NO_ERRORS;
    #endif
}

#endif

//*************************************************************************************************

//***********************************************************************************************//
//                                     SEARCHING AND DELETING                                    //
//***********************************************************************************************//

enum STR_CMP
{
    EQUAL,
    NOT_EQUAL
};

extern int Str_Cmp_SSE (const char *const str_1, const char *const str_2, const size_t len);

static inline void Delete_Beg_ (struct Hash_Table *ht_ptr, const uint64_t hash, struct Node *current)
{
    ht_ptr->array[hash] = (current->next == NULL) ? NULL : current->next;

    free (current->str);
    free (current);
}

static inline void Delete_Mid_ (struct Node *prev, struct Node *current, struct Node *next)
{
    prev->next = next;

    free (current->str);
    free (current);
}

static inline void Delete_End_ (struct Node *prev, struct Node *current)
{
    prev->next = NULL;

    free (current->str);
    free (current);
}

int HT_Delete (struct Hash_Table *ht_ptr, const char *data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, ERROR);

    uint64_t hash = Ded_Hash(data);
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
            if (Str_Cmp_SSE (current->str, data, current->len) == EQUAL)
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
        
        if (Str_Cmp_SSE (current->str, data, current->len) == EQUAL)
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

//*************************************************************************************************
