#include "../Hash_Table.h"

//***********************************************************************************************//
//                                          FILLING                                              //
//***********************************************************************************************//

static int Divide_In_Words (struct Hash_Table *ht_ptr, const char *buffer, const long n_symbs)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (buffer, "const char *buffer",        NULL_PTR, ERROR);
    
    char temp_buffer[50]  = "";
    char clean_buffer[50] = "";

    long temp_i = 0L;
    for (long symb_i = 0; symb_i < n_symbs; symb_i++)
    {
        if ( isalpha (buffer[symb_i]) || buffer[symb_i] == '\'')
            temp_buffer[temp_i++] = buffer[symb_i];
        else if ( isspace (buffer[symb_i]) && temp_i > 0 )
        {
            HT_Insert (ht_ptr, temp_buffer);
            temp_i = 0L;
            
            memmove (temp_buffer, clean_buffer, 50);
        }
    }

    if (temp_i != 0)
        HT_Insert (ht_ptr, temp_buffer);

    return NO_ERRORS;
}

int HT_Fill (struct Hash_Table *ht_ptr, const char *file_name)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);

    FILE *file = Open_File (file_name, "rb");

    long n_symbs = Define_File_Size (file);

    char *buffer = Make_Buffer (file, n_symbs);

    Close_File (file, file_name);

    Divide_In_Words (ht_ptr, buffer, n_symbs);

    return NO_ERRORS;
}

int *HT_Count_Collisions (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr,        "const struct Hash_Table *ht_ptr", NULL_PTR, NULL);

    int *collision_arr = (int *)calloc (ht_ptr->size, sizeof (int));
    MY_ASSERT (collision_arr, "int *collision_arr", NE_MEM, NULL);
    
    for (uint32_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
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

int HT_Print_Collisons (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr,    "const Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    char file_name[29] =  "";

    switch (ht_ptr->hash_func_name)
    {
        case CRINGE_1:
            memmove (file_name, "Hash_Research/Cringe_1.txt",   sizeof "Hash_Research/Cringe_1.txt");
            break;

        case ASCII_HASH:
            memmove (file_name, "Hash_Research/ASCII_Hash.txt", sizeof "Hash_Research/ASCII_Hash.txt");
            break;

        case LEN_HASH:
            memmove (file_name, "Hash_Research/Len_Hash.txt",   sizeof "Hash_Research/Len_Hash.txt");
            break;

        case CHECKSUM:
            memmove (file_name, "Hash_Research/Checksum.txt",   sizeof "Hash_Research/Checksum.txt");
            break;
        
        case DED_HASH:
            memmove (file_name, "Hash_Research/Ded_Hash.txt",   sizeof "Hash_Research/Ded_Hash.txt");
            break;

        case SHA_256:
            memmove (file_name, "Hash_Research/Sha_256.txt",    sizeof "Hash_Research/Sha_256.txt");
            break;
    
        default:
            MY_ASSERT (false, "enum Hash_Func function", UNEXP_VAL, ERROR);
    }

    int *collision_arr = HT_Count_Collisions (ht_ptr);

    FILE *file = Open_File (file_name, "wb");
    
    for (uint32_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
        fprintf (file, "%d\n", collision_arr[cell_i]);

    Close_File (file, file_name);

    free (collision_arr);

    return NO_ERRORS;
}

//***********************************************************************************************//
