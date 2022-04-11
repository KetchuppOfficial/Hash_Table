#include "Hash_Table.h"

#define DUMP 0

int main (void)
{
    #if DEBUG == 1
    OPEN_LOG_FILE;
    #endif
    
    struct Hash_Table *ht_ptr = HT_Ctor (SHA_256, 400);

    HT_Fill (ht_ptr, "Hash_Research/Hamlet.txt");

    #if DUMP == 1
    HT_Dump (ht_ptr);
    #endif

    HT_Print_Collisons (ht_ptr);
    
    HT_Dtor (ht_ptr);
    
    return 0;
}
