#include "Hash_Table.h"
#include "../../C/SHA_256/sha_256.h"

//************************************************************************************************//
//                           CONSTRUCTORS, DESTRUCTORS AND HASH FUNCTIONS                         //
//************************************************************************************************//

//-----------------------------------------------------------------------------
static uint32_t Cringe_1 (const char *data)
{
    return 1;
}

static uint32_t ASCII_Hash (const char *data)
{
    return (uint32_t)data[0];
}

static uint32_t Len_Hash (const char *data)
{
    return strlen (data);
}

static uint32_t Checksum (const char *data)
{
    uint32_t checksum = 0U;
    
    for (int i = 0; data[i] != '\0'; i++)
        checksum += data[i];

    return checksum;
}

static inline uint32_t rotr (uint32_t num, uint32_t shift)
{
    return (num >> shift) | (num << (32 - shift));
}

static uint32_t Ded_Hash (const char *data)
{
    uint32_t hash = data[0];

    for (int i = 0; data[i] != '\0'; i++)
        hash = rotr (hash, 1) ^ data[i + 1];

    return hash;
}
//-----------------------------------------------------------------------------

struct Hash_Table *HT_Ctor (enum Hash_Func function, uint32_t ht_size)
{
    struct Hash_Table *ht_ptr = (struct Hash_Table *)calloc (1, sizeof (struct Hash_Table));
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NE_MEM, NULL);

    ht_ptr->array = (struct Node **)calloc (ht_size, sizeof (struct Node *));
    MY_ASSERT (ht_ptr->array, "ht_ptr->array", NE_MEM, NULL);

    ht_ptr->size = ht_size;

    switch (function)
    {
        case CRINGE_1:
            ht_ptr->hash_func = Cringe_1;
            break;

        case ASCII_HASH:
            ht_ptr->hash_func = ASCII_Hash;
            break;

        case LEN_HASH:
            ht_ptr->hash_func = Len_Hash;
            break;

        case CHECKSUM:
            ht_ptr->hash_func = Checksum;
            break;
        
        case DED_HASH:
            ht_ptr->hash_func = Ded_Hash;
            break;

        case SHA_256:
            ht_ptr->hash_func = sha_256_32;
            break;
    
        default:
            MY_ASSERT (false, "enum Hash_Func function", UNEXP_VAL, NULL);
    }

    return ht_ptr;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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
        if (ht_ptr->array[i] == NULL)
            continue;
        else
            List_Dtor (ht_ptr->array[i]);
    }

    free (ht_ptr->array);
    free (ht_ptr);

    return NO_ERRORS;
}
//***********************************************************************************************//

//***********************************************************************************************//
//                                            INSERTION                                          //
//***********************************************************************************************//

static struct Node *Add_Node (const char *data)
{
    MY_ASSERT (data, "const char *data", NULL_PTR, NULL);

    struct Node *new_node = (struct Node *)calloc (1, sizeof (struct Node));
    MY_ASSERT (new_node, "struct Node *new_node", NE_MEM, NULL);

    size_t len = strlen (data) + 1;
    char *str = (char *)calloc (len, sizeof (char));
    MY_ASSERT (str, "char *str", NE_MEM, NULL);

    memmove (str, data, len);

    new_node->next = NULL;
    new_node->data = str;

    return new_node;
}

int HT_Insert (struct Hash_Table *ht_ptr, const char *data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, ERROR);

    uint32_t hash = (* ht_ptr->hash_func)(data);
    if (hash > ht_ptr->size - 1)
        hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
        ht_ptr->array[hash] = Add_Node (data);
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
    }

    return NO_ERRORS;
}

//*************************************************************************************************

//***********************************************************************************************//
//                                     SEARCHING AND DELETING                                    //
//***********************************************************************************************//

struct Pair HT_Search (const struct Hash_Table *ht_ptr, const char *data)
{
    #if 0
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, (struct Pair)ERROR);
    MY_ASSERT (data,   "const char *data",                NULL_PTR, (struct Pair)ERROR);
    #endif
    
    uint32_t hash = (* ht_ptr->hash_func)(data);
    if (hash > ht_ptr->size - 1)
        hash = hash % ht_ptr->size;

    struct Pair pair = {hash, NOT_FOUND};

    if (ht_ptr->array[hash] == NULL)
        return pair;
    else
    {
        struct Node *current = ht_ptr->array[hash];
        struct Node *next    = current->next;

        for (pair.node_i = 0; next != NULL; pair.node_i++)
        {
            if (strcmp (current->data, data) == 0)
                return pair;
            else
            {
                current = next;
                next = current->next;
            }
        }

        if (strcmp (current->data, data) == 0)
            return pair;
        else
        {
            pair.node_i = NOT_FOUND;
            return pair;
        }
    }
}

static inline void Delete_Beg_ (struct Hash_Table *ht_ptr, const uint32_t hash, struct Node *current)
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

    uint32_t hash = (* ht_ptr->hash_func)(data);
    if (hash > ht_ptr->size - 1)
        hash = hash % ht_ptr->size;

    struct Pair pair = {hash, NOT_FOUND};

    if (ht_ptr->array[hash] == NULL)
        return NOT_FOUND;
    else
    {
        struct Node *prev    = NULL;
        struct Node *current = ht_ptr->array[hash];
        struct Node *next    = current->next;

        for (pair.node_i = 0; next != NULL; pair.node_i++)
        {
            if (strcmp (current->data, data) == 0)
            {               
                if (pair.node_i == 0)
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

        if (strcmp (current->data, data) == 0)
        {
            if (pair.node_i == 0)
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


//***********************************************************************************************//
//                                              DUMP                                             //
//***********************************************************************************************//

static void Print_List (FILE *file, struct Node *node_ptr, const uint32_t cell_i)
{
    struct Node *current = node_ptr;
    struct Node *next = node_ptr->next;
    
    int node_i = 0;
    for ( ; next != NULL; node_i++)
    {
        fprintf (file, "\t\t\tstring%u_%u [label = \"%s\", color = lightskyblue];\n", cell_i, node_i, current->data);

        current = next;
        next = current->next;
    }

    fprintf (file, "\t\t\tstring%u_%d [label = \"%s\", color = lightskyblue];\n", cell_i, node_i, current->data);
}

static void Print_HT (FILE *file, const struct Hash_Table *ht_ptr)
{
    for (uint32_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
    {
        if (ht_ptr->array[cell_i] != NULL)
        {
            fprintf (file, "\t\tsubgraph bucket%u\n"
                           "\t\t{\n"
                           "\t\t\trankdir=TB\n", cell_i);
            
            fprintf (file, "\t\t\tnode%u [label = \"[%u]\", fillcolor = aquamarine];\n", cell_i, cell_i);
            Print_List (file, ht_ptr->array[cell_i], cell_i);

            fprintf (file, "\t\t}\n\n");
        }
        else
            fprintf (file, "\t\tnode%u [label = \"[%u]\", fillcolor = aquamarine];\n\n", cell_i, cell_i);
    }
}

static void Print_Arrows (FILE *file, const struct Hash_Table *ht_ptr)
{
    for (uint32_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
    {
        if (cell_i < ht_ptr->size - 1)
            fprintf (file, "\t\tnode%u -> node%u [color = red, constraint = false, arrowhead = none]\n", cell_i, cell_i + 1);
        
        if (ht_ptr->array[cell_i] != NULL)
        {
            fprintf (file, "\t\tnode%u -> string%u_%d [color = navy];\n", cell_i, cell_i, 0);

            struct Node *current = ht_ptr->array[cell_i];
            struct Node *next = current->next;
            
            for (int node_i = 0; next != NULL; node_i++)
            {
                fprintf (file, "\t\tstring%u_%d -> string%u_%d [color = navy];\n", 
                               cell_i, node_i, cell_i, node_i + 1);

                current = next;
                next = current->next;
            }            
        }

        fprintf (file, "\n");
    }
}

int HT_Dump (const struct Hash_Table *ht_ptr)
{
    FILE *file = Open_File ("Dump/Dump.dot", "wb");

    fprintf (file, "digraph Hash_Table\n"
                         "{\n"
                                "\tgraph [dpi = 200]\n"
                                "\tsplines = ortho\n"
                                "\tnode [shape = box, style = filled];\n"
                                "\tsize [shape = record, fillcolor = springgreen, label = \"<0> SIZE | <1> %u\"];\n"
                                "\tsubgraph Array\n"
                                "\t{\n"
                                    "\t\tbgcolor = gray70;\n"
                                    "\t\trankdir = LR;\n\n", ht_ptr->size);

    Print_HT (file, ht_ptr);
    Print_Arrows (file, ht_ptr);

    fprintf (file, "\t}\n}\n");

    Close_File (file, "Dump/Dump.dot");

    system ("dot -Tpng Dump/Dump.dot -o Dump/Dump.png");

    return NO_ERRORS;
}

//***********************************************************************************************//
