#include "../Not_Optimized/Hash_Table.h"

#define MAX_WORD_LEN 64

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                          FILLING                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static inline int Insert_Word (struct Hash_Table *ht_ptr, char *const str, const long letter_i)
{
    str[letter_i] = '\0';
    int node_i = HT_Search (ht_ptr, str);

    if (node_i == NOT_FOUND)
    {
        #if DEBUG == 0
        HT_Insert (ht_ptr, str);
        #elif DEBUG == 1
        int ret_val = HT_Insert (ht_ptr, str);
        MY_ASSERT (ret_val == NO_ERRORS, "HT_Insert ()", FUNC_ERROR, ERROR);
        #endif

        return 1;
    }

    return 0;
}

static void Divide_In_Words (struct Hash_Table *ht_ptr, const char *buffer, const long n_symbs)
{  
    char str[MAX_WORD_LEN] = "";

    #if DEBUG == 1
    int n_words = 0;
    #endif

    long letter_i = 0L;
    for (long symb_i = 0L; symb_i < n_symbs; symb_i++)
    {      
        if ( isalpha (buffer[symb_i]) || buffer[symb_i] == '\'')
            str[letter_i++] = buffer[symb_i];
        else if (letter_i > 0L)
        {
            n_words += Insert_Word (ht_ptr, str, letter_i);
            letter_i = 0L;
        }
    }

    if (letter_i > 0L)
        n_words += Insert_Word (ht_ptr, str, letter_i);

    #if DEBUG == 1
    printf ("n_words = %d\n", n_words);
    #endif
}

int HT_Fill (struct Hash_Table *ht_ptr, const char *file_name)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (ht_ptr, "const char *file_name",     NULL_PTR, ERROR);

    FILE *file = Open_File (file_name, "rb");
    MY_ASSERT (file, "Open_File ()", FUNC_ERROR, ERROR);

    long n_symbs = Define_File_Size (file);
    MY_ASSERT (n_symbs != ERROR, "Define_File_Size ()", FUNC_ERROR, ERROR);

    char *buffer = Make_Buffer (file, n_symbs);
    MY_ASSERT (buffer, "Make_Buffer ()", FUNC_ERROR, ERROR);

    Close_File (file, file_name);

    Divide_In_Words (ht_ptr, buffer, n_symbs);

    free (buffer);

    return NO_ERRORS;
}

static size_t *HT_Count_Collisions (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, NULL);

    size_t *collision_arr = (size_t *)calloc (ht_ptr->size, sizeof (size_t));
    MY_ASSERT (collision_arr, "size_t *collision_arr", NE_MEM, NULL);
    
    for (uint64_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
    {
        if (ht_ptr->array[cell_i] != NULL)
        {
            struct Node *current = ht_ptr->array[cell_i];
            struct Node *next = current->next;
            
            size_t node_i = 0;
            for ( ; next != NULL; node_i++)
            {
                current = next;
                next = current->next;
            }

            collision_arr[cell_i] = node_i + 1;
        }
    }

    return collision_arr;
}

static const char names_arr[][32] = 
{
    "../Hash_Research/Cringe_1.txt",
    "../Hash_Research/ASCII_Hash.txt",
    "../Hash_Research/Len_Hash.txt",
    "../Hash_Research/Checksum.txt",
    "../Hash_Research/Ded_Hash.txt",
    "../Hash_Research/Sha_256.txt",
};

int HT_Show_Collisons (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    char file_name[32] = "";

    switch (ht_ptr->hash_func_name)
    {
        case CRINGE_1:
            memcpy (file_name, names_arr[CRINGE_1],   sizeof names_arr[CRINGE_1]);
            break;

        case ASCII_HASH:
            memcpy (file_name, names_arr[ASCII_HASH], sizeof names_arr[ASCII_HASH]);
            break;

        case LEN_HASH:
            memcpy (file_name, names_arr[LEN_HASH],   sizeof names_arr[LEN_HASH]);
            break;

        case CHECKSUM:
            memcpy (file_name, names_arr[CHECKSUM],   sizeof names_arr[CHECKSUM]);
            break;
        
        case DED_HASH:
            memcpy (file_name, names_arr[DED_HASH],   sizeof names_arr[DED_HASH]);
            break;

        case SHA_256:
            memcpy (file_name, names_arr[SHA_256],    sizeof names_arr[SHA_256]);
            break;
    
        default:
            MY_ASSERT (false, "enum Hash_Func function", UNEXP_VAL, ERROR);
    }

    size_t *collision_arr = HT_Count_Collisions (ht_ptr);
    MY_ASSERT (collision_arr, "HT_Count_Collisions ()", FUNC_ERROR, ERROR);
    
    FILE *file = Open_File (file_name, "wb");
    MY_ASSERT (file, "Open_File ()", FUNC_ERROR, ERROR);

    for (uint64_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
        fprintf (file, "%lu\t %zd\n", cell_i, collision_arr[cell_i]);

    Close_File (file, file_name);

    free (collision_arr);

    return NO_ERRORS;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
