#include "Hash_Table_V4.h"

#define MAX_WORD_LEN 64

//***********************************************************************************************//
//                                          FILLING                                              //
//***********************************************************************************************//

#if DEBUG == 1
static inline int Insert_Word (struct Hash_Table *ht_ptr, char *const str, const long letter_i)
#elif DEBUG == 0
static inline void Insert_Word (struct Hash_Table *ht_ptr, char *const str, const long letter_i)
#endif
{
    if (HT_Search (ht_ptr, str) == NOT_FOUND)
    {
        #if DEBUG == 0
        HT_Insert (ht_ptr, str);
        #elif DEBUG == 1
        int ret_val = HT_Insert (ht_ptr, str);
        MY_ASSERT (ret_val != ERROR, "HT_Insert ()", FUNC_ERROR, ERROR);
        return 1;
        #endif
    }

    #if DEBUG == 1
    return 0;
    #endif
}

static void Divide_In_Words (struct Hash_Table *ht_ptr, const char *buffer, const long n_symbs)
{
    char str[MAX_WORD_LEN] = "";
    char empty[MAX_WORD_LEN] = "";

    #if DEBUG == 1
    int n_words = 0;
    #endif

    long letter_i = 0L;
    for (long symb_i = 0L; symb_i < n_symbs; symb_i++)
    {      
        if ( ('a' <= buffer[symb_i] && buffer[symb_i] <= 'z') || 
             ('A' <= buffer[symb_i] && buffer[symb_i] <= 'Z') || 
                                       buffer[symb_i] == '\'')
        {
            str[letter_i++] = buffer[symb_i];
        }
        else if (letter_i > 0L)
        {
            #if DEBUG == 1
            n_words += Insert_Word (ht_ptr, str, letter_i);
            #elif DEBUG == 0
            Insert_Word (ht_ptr, str, letter_i);
            #endif

            letter_i = 0L;
            memcpy (str, empty, MAX_WORD_LEN);
        }
    }

    if (letter_i > 0L)
    {
        #if DEBUG == 1
        n_words += Insert_Word (ht_ptr, str, letter_i);
        #elif DEBUG == 0
        Insert_Word (ht_ptr, str, letter_i);
        #endif
    }
    
    #if DEBUG == 1
    printf ("n_words = %d\n", n_words);
    #endif
}

#if DEBUG == 1
int HT_Fill (struct Hash_Table *ht_ptr, const char *file_name)
#elif DEBUG == 0
void HT_Fill (struct Hash_Table *ht_ptr, const char *file_name)
#endif
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (ht_ptr, "const char *file_name",     NULL_PTR, ERROR);

    FILE *file = Open_File (file_name, "rb");

    long n_symbs = Define_File_Size (file);
    MY_ASSERT (n_symbs != ERROR, "Define_File_Size ()", FUNC_ERROR, ERROR);

    char *buffer = Make_Buffer (file, n_symbs);
    MY_ASSERT (buffer, "Make_Buffer ()", FUNC_ERROR, ERROR);

    Close_File (file, file_name);

    Divide_In_Words (ht_ptr, buffer, n_symbs);

    free (buffer);

    #if DEBUG == 1
    return NO_ERRORS;
    #endif
}

//***********************************************************************************************//
