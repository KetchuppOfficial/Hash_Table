#include "Hash_Table.h"

int main (void)
{
    struct Hash_Table *ht_ptr = HT_Ctor (SHA_256, 10);

    HT_Insert (ht_ptr, "It will be legendary!");
    HT_Insert (ht_ptr, "Awesome");
    HT_Insert (ht_ptr, "Suit up!");
    HT_Insert (ht_ptr, "Bazinga!");
    HT_Insert (ht_ptr, "Come with me if you want to live");
    HT_Insert (ht_ptr, "I'll be back");
    HT_Insert (ht_ptr, "May the Force be with you");
    HT_Insert (ht_ptr, "How you doin'?");
    HT_Insert (ht_ptr, "Live long and prosper");
    HT_Insert (ht_ptr, "Space, the final frontier...");
    HT_Insert (ht_ptr, "To the Batmobile!");
    HT_Insert (ht_ptr, "Hasta la vista, baby");
    HT_Insert (ht_ptr, "I love the smell of napalm in the morning");
    HT_Insert (ht_ptr, "The name is Bond, James Bond");

    HT_Delete (ht_ptr, "I'll be back");

    HT_Dump (ht_ptr);
    
    HT_Dtor (ht_ptr);
    
    return 0;
}