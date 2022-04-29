/*
    DESCRIPTION
    ~~~~~~~~~~~

    Hash function Ded_Hash () was implemented in the assembly language

    229 246 310 clock signals
*/

#include "Hash_Table_V1.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const char *text = "../../The Lord of the Rings.txt";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int main (void)
{
    #if DEBUG == 1
    OPEN_LOG_FILE;
    #endif
    
    struct Hash_Table *ht_ptr = HT_Ctor (DED_HASH, 2000);

    HT_Fill (ht_ptr, text);

    HT_Dtor (ht_ptr);

    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
