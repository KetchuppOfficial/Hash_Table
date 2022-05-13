#include "../Hash_Table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
const char dump_file_dir[]  = "../Dump/";

const char dot_file_name[]  = "Dump.dot";

const char dump_file_name[] = "Dump.png";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                              DUMP                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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

static inline char *Make_File_Name (const char *dir, const char *name)
{
    char *path = (char *)calloc (strlen (dir) + strlen (name) + 1, sizeof (char));
    strcpy (path, dir);
    strcat (path, name);
    
    return path;
}

static inline char *Make_System_String (const char *dot, const char *img)
{
    char *string = (char *)calloc (15 + strlen (dot) + strlen (img), sizeof (char));

    sprintf (string, "dot -Tpng %s -o %s", dot, img);

    return string;
}

int HT_Dump (const struct Hash_Table *ht_ptr)
{
    MY_ASSERT (ht_ptr, "const struct Hash_Table *ht_ptr", NULL_PTR, ERROR);

    char *dot_file_path  = Make_File_Name (dump_file_dir, dot_file_name);
    char *dump_file_path = Make_File_Name (dump_file_dir, dump_file_name);

    FILE *file = Open_File (dot_file_path, "wb");
    MY_ASSERT (file, "Open_File ()", FUNC_ERROR, ERROR);

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

    Print_HT     (file, ht_ptr);
    Print_Arrows (file, ht_ptr);
    fprintf      (file, "\t}\n}\n");

    Close_File (file, dot_file_path);

    char *sys_string = Make_System_String (dot_file_path, dump_file_path);
    system (sys_string);

    free (dot_file_path);
    free (dump_file_path);
    free (sys_string);

    return NO_ERRORS;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//