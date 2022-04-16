#define DEBUG 1
#define DUMP 0

#include "Hash_Table.h"

int main (void)
{
    #if DEBUG == 1
    OPEN_LOG_FILE;
    #endif
    
    struct Hash_Table *ht_ptr = HT_Ctor (SHA_256, 2048);

    HT_Fill (ht_ptr, "Hash_Research/The Lord of the Rings.txt");

    #if DUMP == 1
    HT_Dump (ht_ptr);
    #endif

    HT_Print_Collisons (ht_ptr);
    
    HT_Dtor (ht_ptr);
    printf ("time = %f\n", 1.0 * clock () / CLOCKS_PER_SEC);
    
    return 0;
}
