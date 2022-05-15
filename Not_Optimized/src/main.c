#include "../Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const char *text = "../The Lord of the Rings.txt";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int main (void)
{
    #ifdef DEBUG
    OPEN_LOG_FILE;
    #endif

    struct Hash_Table *ht_ptr = HT_Ctor (CRINGE_1, 2000);
    HT_Fill (ht_ptr, text);
    HT_Show_Collisons (ht_ptr);
    HT_Dtor (ht_ptr);
    
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
