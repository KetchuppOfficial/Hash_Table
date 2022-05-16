#include "../Hash_Table.h"

#define MAX_WORD_LEN 64

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                          FILLING                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static inline char *Insert_Word (struct Hash_Table *ht_ptr, char *const str, const long letter_i)
{
    str[letter_i] = '\0';
    int node_i = HT_Search (ht_ptr, str);

    char *new_str = NULL;
    if (node_i == NOT_FOUND)
    {
        new_str = HT_Insert (ht_ptr, str);
        MY_ASSERT (new_str, "HT_Insert ()", FUNC_ERROR, NULL);
    }

    return new_str;
}

static char **Divide_In_Words (struct Hash_Table *ht_ptr, const char *buffer, const long n_symbs, int *n_words)
{  
    char str[MAX_WORD_LEN] = "";

    char **words_arr = (char **)calloc (n_symbs, sizeof (char *));
    MY_ASSERT (words_arr, "char **word_arr", NE_MEM, NULL);
    int word_i = 0;
    
    long letter_i = 0L;
    for (long symb_i = 0L; symb_i < n_symbs; symb_i++)
    {      
        if ( isalpha (buffer[symb_i]) || buffer[symb_i] == '\'')
            str[letter_i++] = buffer[symb_i];
        else if (letter_i > 0L)
        {            
            words_arr[word_i] = Insert_Word (ht_ptr, str, letter_i);
            
            if (words_arr[word_i])
                (word_i)++;

            letter_i = 0L;
        }
    }

    if (letter_i > 0L)
    {
        words_arr[word_i] = Insert_Word (ht_ptr, str, letter_i);
            
        if (words_arr[word_i])
            word_i++;
    }

    words_arr = realloc (words_arr, word_i * sizeof (char *));
    *n_words = word_i;

    #ifdef DEBUG
    printf ("n_words = %d\n", word_i);
    #endif

    return words_arr;
}

char **HT_Fill (struct Hash_Table *ht_ptr, const char *file_name, int *n_words)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, NULL);
    MY_ASSERT (ht_ptr, "const char *file_name",     NULL_PTR, NULL);

    FILE *file = Open_File (file_name, "rb");
    MY_ASSERT (file, "Open_File ()", FUNC_ERROR, NULL);

    long n_symbs = Define_File_Size (file);
    MY_ASSERT (n_symbs != ERROR, "Define_File_Size ()", FUNC_ERROR, NULL);

    char *buffer = Make_Buffer (file, n_symbs);
    MY_ASSERT (buffer, "Make_Buffer ()", FUNC_ERROR, NULL);

    Close_File (file, file_name);

    char **words_arr = Divide_In_Words (ht_ptr, buffer, n_symbs, n_words);
    free (buffer);

    return words_arr;
}

static size_t *HT_Count_Collisions (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, NULL);

    size_t *collision_arr = (size_t *)calloc (ht_ptr->size, sizeof (size_t));
    MY_ASSERT (collision_arr, "size_t *collision_arr", NE_MEM, NULL);
    
    for (uint32_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
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

static const char names_arr[][sizeof "ASCII-Hash"] = 
{
    "Cringe-1",
    "ASCII-Hash",
    "Len-Hash",
    "Checksum",
    "Ded-Hash",
    "CRC-32",
    "SHA-256",
};

static inline void Print_Histogram (const char *script_path, const char *graph_path, const char *graph_name)
{
    const char *first_part = "python3 ";

    int first_len  = strlen (first_part);
    int script_len = strlen (script_path);
    int path_len   = strlen (graph_path);
    int name_len   = strlen (graph_name);

    char *system_arg = (char *)calloc (first_len + script_len + path_len + name_len + 3, sizeof (char));
    
    memcpy (system_arg, first_part, first_len);
    strcat (system_arg, script_path);
    strcat (system_arg, " ");
    strcat (system_arg, graph_path);
    strcat (system_arg, " ");
    strcat (system_arg, graph_name);

    printf ("%s\n", system_arg);

    system (system_arg);

    free (system_arg);
}

static char *Name_Gen (const char *path, const char *name, const char *extension)
{
    size_t file_name_len = strlen (path) + strlen (name) + strlen (extension) + 1;
    char *file_name = (char *)calloc (file_name_len, sizeof (char));
    
    strcat (file_name, path);
    strcat (file_name, name);
    strcat (file_name, extension);

    return file_name;
}

int HT_Show_Collisons (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const Hash_Table *ht_ptr", NULL_PTR, ERROR);

    size_t *collision_arr = HT_Count_Collisions (ht_ptr);
    MY_ASSERT (collision_arr, "HT_Count_Collisions ()", FUNC_ERROR, ERROR);

    const char path[] = "./Hash_Research/";
    const char *hash_func_name = names_arr[ht_ptr->hash_func_name];
    const char extension[] = ".txt";

    char *file_name = Name_Gen (path, hash_func_name, extension);    

    FILE *file = Open_File (file_name, "wb");
    MY_ASSERT (file, "Open_File ()", FUNC_ERROR, ERROR);

    for (uint32_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
        fprintf (file, "%zd\n", collision_arr[cell_i]);

    Close_File (file, file_name);
    free (file_name);

    Print_Histogram ("./src/Histogram.py", path, hash_func_name);

    free (collision_arr);

    return NO_ERRORS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
