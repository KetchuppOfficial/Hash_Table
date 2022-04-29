#include "Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const char *text = "../The Lord of the Rings.txt";
const char *test = "test.txt";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int main (void)
{
    #if DEBUG == 1
    OPEN_LOG_FILE;
    #endif

    struct Hash_Table *ht_ptr = HT_Ctor (SHA_256, 20);

    HT_Fill (ht_ptr, test);

    HT_Show_Collisons (ht_ptr);
    
    #if DUMP == 1
    HT_Dump (ht_ptr);
    #endif

    HT_Dtor (ht_ptr);
    
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
