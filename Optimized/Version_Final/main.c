#include "Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
const char *text = "../The Lord of the Rings.txt";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int main (void)
{
    #if DEBUG == 1
    OPEN_LOG_FILE;
    #endif
    
    struct Hash_Table *ht_ptr = HT_Ctor (DED_HASH, 2000);

    HT_Fill (ht_ptr, text);

    #if 0
    HT_Delete_All (ht_ptr);
    #endif

    HT_Dtor (ht_ptr);

    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
