#include "../Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const char *text = "../../The Lord of the Rings.txt";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int main (void)
{
    #ifdef DEBUG
    Open_Log_File ("Version_0");
    #endif

    struct Hash_Table *ht_ptr = HT_Ctor (2000);
    
    int n_words = 0;
    char **words_arr = HT_Fill (ht_ptr, text, &n_words);

    HT_Test (ht_ptr, words_arr, n_words, 5000);

    free (words_arr);

    HT_Dtor (ht_ptr);
    
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

