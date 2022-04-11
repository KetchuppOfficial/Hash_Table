#include "Hash_Table.h"

int main (void)
{
    #if DEBUG == 1
    OPEN_LOG_FILE;
    #endif
    
    struct Hash_Table *ht_ptr = HT_Ctor (SHA_256, 20);

    HT_Fill (ht_ptr, "Hash_Research/Test.txt");

    HT_Dump (ht_ptr);

    HT_Print_Collisons (ht_ptr);
    
    HT_Dtor (ht_ptr);
    
    return 0;
}
