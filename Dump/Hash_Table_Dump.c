#include "../Hash_Table.h"

//***********************************************************************************************//
//                                              DUMP                                             //
//***********************************************************************************************//

static void Print_List (FILE *file, struct Node *node_ptr, const uint64_t cell_i)
{
    struct Node *current = node_ptr;
    struct Node *next = node_ptr->next;
    
    int node_i = 0;
    for ( ; next != NULL; node_i++)
    {
        fprintf (file, "\t\t\tstring%lu_%d [label = \"%s\", color = lightskyblue];\n", cell_i, node_i, current->data);

        current = next;
        next = current->next;
    }

    fprintf (file, "\t\t\tstring%lu_%d [label = \"%s\", color = lightskyblue];\n", cell_i, node_i, current->data);
}

static void Print_HT (FILE *file, const struct Hash_Table *ht_ptr)
{
    for (uint64_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
    {        
        if (ht_ptr->array[cell_i] != NULL)
        {
            fprintf (file, "\t\tsubgraph bucket%lu\n"
                           "\t\t{\n"
                           "\t\t\trankdir=TB\n", cell_i);
            
            fprintf (file, "\t\t\tnode%lu [label = \"[%lu]\", fillcolor = aquamarine];\n", cell_i, cell_i);
            Print_List (file, ht_ptr->array[cell_i], cell_i);

            fprintf (file, "\t\t}\n\n");
        }
        else
            fprintf (file, "\t\tnode%lu [label = \"[%lu]\", fillcolor = aquamarine];\n\n", cell_i, cell_i);
    }
}

static void Print_Arrows (FILE *file, const struct Hash_Table *ht_ptr)
{
    for (uint64_t cell_i = 0; cell_i < ht_ptr->size; cell_i++)
    {
        if (cell_i < ht_ptr->size - 1)
            fprintf (file, "\t\tnode%lu -> node%lu [color = red, constraint = false, arrowhead = none]\n", cell_i, cell_i + 1);
        
        if (ht_ptr->array[cell_i] != NULL)
        {
            fprintf (file, "\t\tnode%lu -> string%lu_%d [color = navy];\n", cell_i, cell_i, 0);

            struct Node *current = ht_ptr->array[cell_i];
            struct Node *next = current->next;
            
            for (int node_i = 0; next != NULL; node_i++)
            {
                fprintf (file, "\t\tstring%lu_%d -> string%lu_%d [color = navy];\n", 
                               cell_i, node_i, cell_i, node_i + 1);

                current = next;
                next = current->next;
            }            
        }

        fprintf (file, "\n");
    }
}

int HT_Dump (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "cosnt struct Hash_Table *ht_ptr", NULL_PTR, ERROR);
    
    FILE *file = Open_File ("Dump/Dump.dot", "wb");

    fprintf (file, "digraph Hash_Table\n"
                         "{\n"
                                "\tgraph [dpi = 200]\n"
                                "\tsplines = ortho\n"
                                "\tnode [shape = box, style = filled];\n"
                                "\tsize [shape = record, fillcolor = springgreen, label = \"<0> SIZE | <1> %lu\"];\n"
                                "\tsubgraph Array\n"
                                "\t{\n"
                                    "\t\tbgcolor = gray70;\n"
                                    "\t\trankdir = LR;\n\n", ht_ptr->size);

    Print_HT (file, ht_ptr);
    Print_Arrows (file, ht_ptr);

    fprintf (file, "\t}\n}\n");

    Close_File (file, "Dump/Dump.dot");

    system ("dot -Tpng Dump/Dump.dot -o Dump/Dump.png");

    return NO_ERRORS;
}

//***********************************************************************************************//