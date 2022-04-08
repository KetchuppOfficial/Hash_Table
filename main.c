#include "Hash_Table.h"

int main (void)
{
    struct Hash_Table *ht_ptr = HT_Ctor (SHA_256);

    HT_Insert (ht_ptr, "It will be legendary!");

    HT_Dtor (ht_ptr);
    
    return 0;
}