#include "../Hash_Table.h"

#define MAX_WORD_LEN 64

//***********************************************************************************************//
//                                          FILLING                                              //
//***********************************************************************************************//

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

    long n_symbs = 0;
    char *buffer = Make_File_Buffer (file_name, &n_symbs);

    char **words_arr = Divide_In_Words (ht_ptr, buffer, n_symbs, n_words);
    free (buffer);

    return words_arr;
}

void HT_Test (const struct Hash_Table *ht_ptr, char **words_arr, const int n_words, const int n_tests)
{
    for (int i = 0; i < n_tests; i++)
        for (int word_i = 0; word_i < n_words; word_i++)
            HT_Search (ht_ptr, words_arr[word_i]);
}

//***********************************************************************************************//
