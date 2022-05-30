#include "../Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const char *text = "../The Lord of the Rings.txt";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int main (void)
{
    #ifdef DEBUG
    Open_Log_File ("Not_Optimized");
    #endif

    struct Hash_Table *ht_ptr = HT_Ctor (CRC_32, 2000);
    
    int n_words = 0;
    char **words_arr = HT_Fill (ht_ptr, text, &n_words);

    #if 0
    HT_Test (ht_ptr, words_arr, n_words, 16485);
    #endif

    free (words_arr);

    #if 1
    HT_Show_Collisons (ht_ptr);
    #endif

    HT_Dtor (ht_ptr);
    
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
