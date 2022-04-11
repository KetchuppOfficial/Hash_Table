#include "../Hash_Table.h"

//***********************************************************************************************//
//                                          FILLING                                              //
//***********************************************************************************************//

static int Divide_In_Words (struct Hash_Table *ht_ptr, const char *buffer, const long n_symbs)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (buffer, "const char *buffer",        NULL_PTR, ERROR);
    MY_ASSERT (buffer, "const long n_symbs",        POS_VAL,  ERROR);
    
    char temp_buffer[50]  = "";
    char clean_buffer[50] = "";

    long temp_i = 0L;
    for (long symb_i = 0; symb_i < n_symbs; symb_i++)
    {
        if ( isalpha (buffer[symb_i]) || buffer[symb_i] == '\'')
            temp_buffer[temp_i++] = buffer[symb_i];
        else if ( isspace (buffer[symb_i]) && temp_i > 0 )
        {
            struct Pair pair = HT_Search (ht_ptr, temp_buffer);

            if (pair.node_i == NOT_FOUND)
            {
                #if DEBUG == 0
                HT_Insert (ht_ptr, temp_buffer);
                #elif DEBUG == 1
                int ret_val = HT_Insert (ht_ptr, temp_buffer);
                MY_ASSERT (ret_val == NO_ERRORS, "HT_Insert ()", FUNC_ERROR, ERROR);
                #endif
            }

            temp_i = 0L;
            memmove (temp_buffer, clean_buffer, 50);
        }
    }

    if (temp_i != 0)
    {
        #if DEBUG == 0
        HT_Insert (ht_ptr, temp_buffer);
        #elif DEBUG == 1
        int ret_val = HT_Insert (ht_ptr, temp_buffer);
        MY_ASSERT (ret_val == NO_ERRORS, "HT_Insert ()", FUNC_ERROR, ERROR);
        #endif
    }

    return NO_ERRORS;
}

int HT_Fill (struct Hash_Table *ht_ptr, const char *file_name)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (ht_ptr, "const char *file_name",     NULL_PTR, ERROR);

    FILE *file = Open_File (file_name, "rb");

    long n_symbs = Define_File_Size (file);
    MY_ASSERT (n_symbs != ERROR, "Define_File_Size ()", FUNC_ERROR, ERROR);

    char *buffer = Make_Buffer (file, n_symbs);

    Close_File (file, file_name);

    #if DEBUG == 0
    Divide_In_Words (ht_ptr, buffer, n_symbs);
    #elif DEBUG == 1
    int ret_val = Divide_In_Words (ht_ptr, buffer, n_symbs);
    MY_ASSERT (ret_val != ERROR, "Divide_In_Words ()", FUNC_ERROR, ERROR);
    #endif

    return NO_ERRORS;
}

int *HT_Count_Collisions (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, NULL);

    int *collision_arr = (int *)calloc (ht_ptr->size, sizeof (int));
    MY_ASSERT (collision_arr, "int *collision_arr", NE_MEM, NULL);
    
    for (uint64_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
    {
        if (ht_ptr->array[cell_i] != NULL)
        {
            struct Node *current = ht_ptr->array[cell_i];
            struct Node *next = current->next;
            
            int node_i = 0;
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

static const char names_arr[][29] = 
{
    "Hash_Research/Cringe_1.txt",
    "Hash_Research/ASCII_Hash.txt",
    "Hash_Research/Len_Hash.txt",
    "Hash_Research/Checksum.txt",
    "Hash_Research/Ded_Hash.txt",
    "Hash_Research/Sha_256.txt",
};

int HT_Print_Collisons (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    char file_name[29] =  "";

    switch (ht_ptr->hash_func_name)
    {
        case CRINGE_1:
            memmove (file_name, names_arr[CRINGE_1],   sizeof names_arr[CRINGE_1]);
            break;

        case ASCII_HASH:
            memmove (file_name, names_arr[ASCII_HASH], sizeof names_arr[ASCII_HASH]);
            break;

        case LEN_HASH:
            memmove (file_name, names_arr[LEN_HASH],   sizeof names_arr[LEN_HASH]);
            break;

        case CHECKSUM:
            memmove (file_name, names_arr[CHECKSUM],   sizeof names_arr[CHECKSUM]);
            break;
        
        case DED_HASH:
            memmove (file_name, names_arr[DED_HASH],   sizeof names_arr[DED_HASH]);
            break;

        case SHA_256:
            memmove (file_name, names_arr[SHA_256],    sizeof names_arr[SHA_256]);
            break;
    
        default:
            MY_ASSERT (false, "enum Hash_Func function", UNEXP_VAL, ERROR);
    }

    int *collision_arr = HT_Count_Collisions (ht_ptr);
    MY_ASSERT (collision_arr, "HT_Count_Collisions ()", FUNC_ERROR, ERROR);

    FILE *file = Open_File (file_name, "wb");
    
    for (uint64_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
        fprintf (file, "%lu\t %d\n", cell_i, collision_arr[cell_i]);

    Close_File (file, file_name);

    free (collision_arr);

    return NO_ERRORS;
}

//***********************************************************************************************//
