#include "../Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                         HASH FUNCTIONS                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static uint32_t Cringe_1 (const char *data)
{
    return 1;
}

static uint32_t ASCII_Hash (const char *data)
{
    return (uint32_t)data[0];
}

static uint32_t Len_Hash (const char *data)
{
    return strlen (data);
}

static uint32_t Checksum (const char *data)
{
    uint32_t checksum = 0U;
    
    for (int i = 0; data[i] != '\0'; i++)
        checksum += data[i];

    return checksum;
}

static inline uint32_t ror_32 (uint32_t num, uint32_t shift)
{
    return (num >> shift) | (num << (__CHAR_BIT__ * sizeof (uint32_t) - shift));
}

static uint32_t Ded_Hash (const char *data)
{
    uint32_t hash = data[0];

    for (int i = 0; data[i] != '\0'; i++)
        hash = ror_32 (hash, 1) ^ data[i];

    return hash;
}

static const uint32_t crc_lookup_table[256] =
{
 	0x00000000, 0xF26B8303, 0xE13B70F7, 0x1350F3F4, 0xC79A971F, 0x35F1141C, 0x26A1E7E8, 0xD4CA64EB, 
	0x8AD958CF, 0x78B2DBCC, 0x6BE22838, 0x9989AB3B, 0x4D43CFD0, 0xBF284CD3, 0xAC78BF27, 0x5E133C24, 
	0x105EC76F, 0xE235446C, 0xF165B798, 0x030E349B, 0xD7C45070, 0x25AFD373, 0x36FF2087, 0xC494A384, 
	0x9A879FA0, 0x68EC1CA3, 0x7BBCEF57, 0x89D76C54, 0x5D1D08BF, 0xAF768BBC, 0xBC267848, 0x4E4DFB4B, 
	0x20BD8EDE, 0xD2D60DDD, 0xC186FE29, 0x33ED7D2A, 0xE72719C1, 0x154C9AC2, 0x061C6936, 0xF477EA35, 
	0xAA64D611, 0x580F5512, 0x4B5FA6E6, 0xB93425E5, 0x6DFE410E, 0x9F95C20D, 0x8CC531F9, 0x7EAEB2FA, 
	0x30E349B1, 0xC288CAB2, 0xD1D83946, 0x23B3BA45, 0xF779DEAE, 0x05125DAD, 0x1642AE59, 0xE4292D5A, 
	0xBA3A117E, 0x4851927D, 0x5B016189, 0xA96AE28A, 0x7DA08661, 0x8FCB0562, 0x9C9BF696, 0x6EF07595, 
	0x417B1DBC, 0xB3109EBF, 0xA0406D4B, 0x522BEE48, 0x86E18AA3, 0x748A09A0, 0x67DAFA54, 0x95B17957, 
	0xCBA24573, 0x39C9C670, 0x2A993584, 0xD8F2B687, 0x0C38D26C, 0xFE53516F, 0xED03A29B, 0x1F682198, 
	0x5125DAD3, 0xA34E59D0, 0xB01EAA24, 0x42752927, 0x96BF4DCC, 0x64D4CECF, 0x77843D3B, 0x85EFBE38, 
	0xDBFC821C, 0x2997011F, 0x3AC7F2EB, 0xC8AC71E8, 0x1C661503, 0xEE0D9600, 0xFD5D65F4, 0x0F36E6F7, 
	0x61C69362, 0x93AD1061, 0x80FDE395, 0x72966096, 0xA65C047D, 0x5437877E, 0x4767748A, 0xB50CF789, 
	0xEB1FCBAD, 0x197448AE, 0x0A24BB5A, 0xF84F3859, 0x2C855CB2, 0xDEEEDFB1, 0xCDBE2C45, 0x3FD5AF46, 
	0x7198540D, 0x83F3D70E, 0x90A324FA, 0x62C8A7F9, 0xB602C312, 0x44694011, 0x5739B3E5, 0xA55230E6, 
	0xFB410CC2, 0x092A8FC1, 0x1A7A7C35, 0xE811FF36, 0x3CDB9BDD, 0xCEB018DE, 0xDDE0EB2A, 0x2F8B6829, 
	0x82F63B78, 0x709DB87B, 0x63CD4B8F, 0x91A6C88C, 0x456CAC67, 0xB7072F64, 0xA457DC90, 0x563C5F93, 
	0x082F63B7, 0xFA44E0B4, 0xE9141340, 0x1B7F9043, 0xCFB5F4A8, 0x3DDE77AB, 0x2E8E845F, 0xDCE5075C, 
	0x92A8FC17, 0x60C37F14, 0x73938CE0, 0x81F80FE3, 0x55326B08, 0xA759E80B, 0xB4091BFF, 0x466298FC, 
	0x1871A4D8, 0xEA1A27DB, 0xF94AD42F, 0x0B21572C, 0xDFEB33C7, 0x2D80B0C4, 0x3ED04330, 0xCCBBC033, 
	0xA24BB5A6, 0x502036A5, 0x4370C551, 0xB11B4652, 0x65D122B9, 0x97BAA1BA, 0x84EA524E, 0x7681D14D, 
	0x2892ED69, 0xDAF96E6A, 0xC9A99D9E, 0x3BC21E9D, 0xEF087A76, 0x1D63F975, 0x0E330A81, 0xFC588982, 
	0xB21572C9, 0x407EF1CA, 0x532E023E, 0xA145813D, 0x758FE5D6, 0x87E466D5, 0x94B49521, 0x66DF1622, 
	0x38CC2A06, 0xCAA7A905, 0xD9F75AF1, 0x2B9CD9F2, 0xFF56BD19, 0x0D3D3E1A, 0x1E6DCDEE, 0xEC064EED, 
	0xC38D26C4, 0x31E6A5C7, 0x22B65633, 0xD0DDD530, 0x0417B1DB, 0xF67C32D8, 0xE52CC12C, 0x1747422F, 
	0x49547E0B, 0xBB3FFD08, 0xA86F0EFC, 0x5A048DFF, 0x8ECEE914, 0x7CA56A17, 0x6FF599E3, 0x9D9E1AE0, 
	0xD3D3E1AB, 0x21B862A8, 0x32E8915C, 0xC083125F, 0x144976B4, 0xE622F5B7, 0xF5720643, 0x07198540, 
	0x590AB964, 0xAB613A67, 0xB831C993, 0x4A5A4A90, 0x9E902E7B, 0x6CFBAD78, 0x7FAB5E8C, 0x8DC0DD8F, 
	0xE330A81A, 0x115B2B19, 0x020BD8ED, 0xF0605BEE, 0x24AA3F05, 0xD6C1BC06, 0xC5914FF2, 0x37FACCF1, 
	0x69E9F0D5, 0x9B8273D6, 0x88D28022, 0x7AB90321, 0xAE7367CA, 0x5C18E4C9, 0x4F48173D, 0xBD23943E, 
	0xF36E6F75, 0x0105EC76, 0x12551F82, 0xE03E9C81, 0x34F4F86A, 0xC69F7B69, 0xD5CF889D, 0x27A40B9E, 
	0x79B737BA, 0x8BDCB4B9, 0x988C474D, 0x6AE7C44E, 0xBE2DA0A5, 0x4C4623A6, 0x5F16D052, 0xAD7D5351  
};

static inline uint32_t crc_32_ (const uint8_t *data, size_t len)
{
    const uint32_t crc_32_magic_const = 0xFFFFFFFFU;

    uint32_t crc_32 = crc_32_magic_const;

	for (size_t i = 0; i < len; i++)
    {
        uint32_t lookup_i = (uint8_t)crc_32 ^ data[i];
        crc_32 = (crc_32 >> 8) ^ crc_lookup_table[lookup_i];
    }

    crc_32 ^= crc_32_magic_const;
    
    return crc_32;
}

static inline uint32_t crc_32 (const char *data)
{
    return crc_32_ ((uint8_t *)data, strlen (data));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                  CONSTRUCTORS AND DESTRUCTORS                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Hash_Table *HT_Ctor (enum Hash_Func function, const uint32_t ht_size)
{
    MY_ASSERT (ht_size > 0, "const uint32_t ht_size", POS_VAL, NULL);
    
    struct Hash_Table *ht_ptr = (struct Hash_Table *)calloc (1, sizeof (struct Hash_Table));
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NE_MEM, NULL);

    ht_ptr->array = (struct Node **)calloc (ht_size, sizeof (struct Node *));
    MY_ASSERT (ht_ptr->array, "ht_ptr->array", NE_MEM, NULL);

    ht_ptr->size = ht_size;

    ht_ptr->hash_func_name = function;
    switch (function)
    {
        case CRINGE_1:
            ht_ptr->hash_func = Cringe_1;
            break;

        case ASCII_HASH:
            ht_ptr->hash_func = ASCII_Hash;
            break;

        case LEN_HASH:
            ht_ptr->hash_func = Len_Hash;
            break;

        case CHECKSUM:
            ht_ptr->hash_func = Checksum;
            break;
        
        case DED_HASH:
            ht_ptr->hash_func = Ded_Hash;
            break;

        case CRC_32:
            ht_ptr->hash_func = crc_32;
            break;

        case SHA_256:
            ht_ptr->hash_func = sha_256_32;
            break;
    
        default:
            MY_ASSERT (false, "enum Hash_Func function", UNEXP_VAL, NULL);
    }

    return ht_ptr;
}

static int List_Dtor (struct Node *node_ptr)
{
    MY_ASSERT (node_ptr, "struct Node *node_ptr", NULL_PTR, ERROR);

    struct Node *current = node_ptr;
    struct Node *prev    = NULL;
    
    while (current->next != NULL)
    {
        prev = current;
        current = current->next;

        free (prev->data);
        free (prev);
    }

    free (current->data);
    free (current);

    return NO_ERRORS;
}

int HT_Dtor (struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    for (uint32_t i = 0; i < ht_ptr->size; i++)
    {
        if (ht_ptr->array[i] != NULL)
            List_Dtor (ht_ptr->array[i]);
    }

    free (ht_ptr->array);
    free (ht_ptr);

    return NO_ERRORS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                            INSERTION                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static struct Node *Add_Node (const char *data)
{
    MY_ASSERT (data, "const char *data", NULL_PTR, NULL);

    struct Node *new_node = (struct Node *)calloc (1, sizeof (struct Node));
    MY_ASSERT (new_node, "struct Node *new_node", NE_MEM, NULL);

    size_t len = strlen (data);
    char *str = (char *)calloc (len + 1, sizeof (char));
    MY_ASSERT (str, "char *str", NE_MEM, NULL);

    memcpy (str, data, len + 1);

    new_node->next = NULL;
    new_node->data = str;
    new_node->len  = len;

    return new_node;
}

char *HT_Insert (struct Hash_Table *ht_ptr, const char *data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, NULL);
    MY_ASSERT (data,   "const char *data",          NULL_PTR, NULL);

    uint32_t hash = (* ht_ptr->hash_func)(data);
    hash = hash % ht_ptr->size;

    if (ht_ptr->array[hash] == NULL)
    {
        ht_ptr->array[hash] = Add_Node (data);
        MY_ASSERT (ht_ptr->array[hash], "Add_Node ()", FUNC_ERROR, NULL);
    }
    else
    {
        struct Node **list_arr = ht_ptr->array;
        
        struct Node *old_first_node = list_arr[hash];
        list_arr[hash] = Add_Node (data);
        MY_ASSERT (list_arr[hash], "Add_Node ()", FUNC_ERROR, NULL);
        list_arr[hash]->next = old_first_node;
    }

    return ht_ptr->array[hash]->data;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                     SEARCHING AND DELETING                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

static int List_Search (struct Node *const list_ptr, const char *const data)
{
    MY_ASSERT (list_ptr, "struct Node *const list_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,     "const char *const data",      NULL_PTR, ERROR);
    
    struct Node *current = list_ptr;
    struct Node *next    = list_ptr->next;

    int node_i = 0;
    for ( ; next != NULL; node_i++)
    {           
        if (memcmp (current->data, data, current->len + 1) == 0)
            return node_i;
        else
        {
            current = next;
            next = current->next;
        }
    }

    return (memcmp (current->data, data, current->len + 1) == 0) ? node_i : NOT_FOUND;
}

int HT_Search (const struct Hash_Table *const ht_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *const data",          NULL_PTR, ERROR);
    
    uint32_t hash = (* ht_ptr->hash_func)(data);
    hash = hash % ht_ptr->size;

    struct Node *list_ptr = ht_ptr->array[hash];

    if (list_ptr == NULL)
        return NOT_FOUND;
    else
        return List_Search (list_ptr, data);
}

static inline void Delete_Beg_ (struct Hash_Table *const ht_ptr, const uint32_t hash, struct Node *const current)
{
    ht_ptr->array[hash] = (current->next == NULL) ? NULL : current->next;

    free (current->data);
    free (current);
}

static inline void Delete_Mid_ (struct Node *const prev, struct Node *const current, struct Node *const next)
{
    prev->next = next;

    free (current->data);
    free (current);
}

static inline void Delete_End_ (struct Node *const prev, struct Node *const current)
{
    prev->next = NULL;
    
    free (current->data);
    free (current);
}

static int List_Delete (struct Hash_Table *const ht_ptr, const uint32_t hash, struct Node *const list_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr,   "struct Hash_Table *ht_ptr",   NULL_PTR, ERROR);
    MY_ASSERT (list_ptr, "struct Node *const list_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,     "const char *const data",      NULL_PTR, ERROR);
    
    struct Node *prev    = NULL;
    struct Node *current = list_ptr;
    struct Node *next    = current->next;

    int node_i = 0;
    for ( ; next != NULL; node_i++)
    {
        if (memcmp (current->data, data, current->len + 1) == 0)
        {               
            if (node_i == 0)
                Delete_Beg_ (ht_ptr, hash, current);
            else
                Delete_Mid_ (prev, current, next);

            return NO_ERRORS;
        }
        else
        {
            prev    = current;
            current = next;
            next    = current->next;
        }
    }

    if (memcmp (current->data, data, current->len + 1) == 0)
    {
        if (node_i == 0)
            Delete_Beg_ (ht_ptr, hash, current);
        else
            Delete_End_ (prev, current);

        return NO_ERRORS;
    }
    else
        return NOT_FOUND;
}

int HT_Delete (struct Hash_Table *const ht_ptr, const char *const data)
{
    MY_ASSERT (ht_ptr, "struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    MY_ASSERT (data,   "const char *const data",    NULL_PTR, ERROR);

    uint32_t hash = (* ht_ptr->hash_func)(data);
    hash = hash % ht_ptr->size;

    struct Node *list_ptr = ht_ptr->array[hash];

    if (list_ptr == NULL)
        return NOT_FOUND;
    else
        return List_Delete (ht_ptr, hash, list_ptr, data);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
