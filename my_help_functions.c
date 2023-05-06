#ifdef IDENT_C
static const char *const functions_c_Id =
    "$Id$";
#endif

#include "keywords.h"
#include "functions.h"
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

/**
 * Parses a string and removes all character after a numeric character
 * @param the source string
 */
char *trim(char *source)
{
    int i = 0, index = 0;
    int sr_length = strlen(source);
    for (i = 0; i < sr_length; i++)
    {
        if ((source[i] == '_' && i != 0))
        {
            source[index] = '\0';
            break;
        }
        else
            source[index++] = source[i];
    }
    source[index] = '\0';
    return source;
}

/**
 * Determines if a string is reserved keyword
 * @param the string to compare
 * @return whether the string is reserved or not
 */
bool reserved(char *word)
{
    int i;
    for (i = 0; i < RESERVEDNUM; i++)
    {
        if (strcmp(word, RESERVED_WORD[i]) == 0 || strstr(word, "endmodule") != NULL)
        {
            return true;
        }
    }
    return false;
}

/**
 * Determines if end of module
 * @param the string to check
 * @return if end of module or not
 */
bool end_of_module(char *word)
{
    if (strstr(word, "endmodule") != NULL)
        return true;
    return false;
}

/**
 * Determines if a string is a vector of signals
 * @param the string to check
 * @return whether the string is a vector of signals or not
 */
bool signal_vector(char *word)
{
    int i, words_length = strlen(word);

    for (i = 0; i < words_length; i++)
        if (word[i] == ';')
            return true;
    return false;
}

/**
 * Parse a signal vector
 * @param a collection of signals, a collection of tokens, a pointer to the index, a pointer to the number of counts
 */
void parse_signal_vector(char *signal_arr[], char *token[], int *index, int *count)
{
    int v, v1, v2;
    char *sig_vector;                        /*Array to hold tokens for the line*/
    sig_vector = strtok(token[*index], ":"); /*Tokenize the vector to extract vector width*/
    v1 = atoi(sig_vector);                   /*Starting index for the vector*/
    sig_vector = strtok(NULL, ":");
    v2 = atoi(sig_vector); /*Ending index for the vector*/
    (*index)++;            /*Get vector name from the next token*/
    for (v = v2; v <= v1; v++)
    {                                                            /*Add the vector signals to the array of signals*/
        int wordsize = strlen(token[*index]);                    /*Size of the string read from the line*/
        signal_arr[*count] = calloc(wordsize + 1, sizeof(char)); /*Allocating memory for signal string*/
        strcpy(signal_arr[*count], token[*index]);               /*Add the signal name to the array of signals*/
        (*count)++;                                              /*Update the number of signals in the module*/
    }
}

/**
 * Determines if a string is gate
 * @param the string to check
 * @return whether the string is a gate or not
 */
bool gate(char *word)
{
    int i;
    for (i = 0; i < GATESNUM; i++)
        if (strcmp(word, GATE_NAME[i]) == 0)
            return true;
    return false;
}

/**
 * Parses a string and search for the character ';'
 * @param the source string
 * @return whether the character ';' is found or not
 */
bool end_of_line(char *source)
{
    char *pchar = strchr(source, ';');
    return (pchar == NULL) ? false : true;
}

/**
 * Determines if a wire is already created
 * @param the module object, the wire name
 * @return whether the wire is already created or not
 */
bool defined_in(module *m, char *name)
{
    int i = 0;

    for (i = 0; i < m->inputcount; i++)
    {
        if (strcmp(m->input_names[i], name) == 0)
            return true;
    }
    return false;
}

bool defined_out(module *m, char *name)
{
    int i = 0;
    for (i = 0; i < m->outputcount; i++)
    {
        if (strcmp(m->output_names[i], name) == 0)
            return true;
    }

    return false;
}

wire *defined_wire(module *m, char *name)
{
    int i = 0;
    while (m->wires[i] != NULL)
    {
        if (strcmp(m->wires[i]->name, name) == 0)
            return m->wires[i];
        i++;
    }
    return NULL;
}

bool dot_plus_char(char *source)
{
    if (source[0] == '.')
    {
        return true;
    }

    return false;
}

void set_wire_io(module *m, wire *w, char *name, bool in_out_flag)
{

    if (defined_in(m, name)) /*If wire is not already defined and is a input or output of the module*/
    {

        w->input_id = 0;
        w->output_ids[w->num_of_outputs - 1] = m->nodecount;
        return;
    }

    if (defined_out(m, name)) /*If wire is not already defined and is a input or output of the module*/
    {
        if (in_out_flag)
        {
            w->input_id = m->nodecount;
        }
        else
        {
            w->output_ids[w->num_of_outputs - 1] = m->nodecount;
        }
        return;
    }

    if (in_out_flag) // It's a wire
    {
        w->input_id = m->nodecount;
    }
    else
    {
        w->output_ids[w->num_of_outputs - 1] = m->nodecount;
    }
}

/**
 * Create a wire
 * @param the module object, the wire object, the wire type, the wire name
 */
void build_wire_(wire *BackUp_w, module *m, int *index, char *token, bool in_out_flag)
{
    int initial_output_ids_size = 1;
    if (BackUp_w == NULL)
    {
        m->wires[(*index)] = (wire *)calloc(1, sizeof(struct wire_));
        m->wires[(*index)]->output_ids = (int *)calloc(initial_output_ids_size, sizeof(int)); // initial size is 5 and if we need more we'll allocate later
        // init of the ids
        m->wires[(*index)]->input_id = -1; // the gate's id that the wire generated
        m->wires[(*index)]->output_ids[0] = -1;
        m->wires[(*index)]->id = (*index); // wires id

        if (!in_out_flag)
        {
            m->wires[(*index)]->num_of_outputs = 1;
            m->nodes[m->nodecount - 1]->input_wires_ids[m->nodes[m->nodecount - 1]->num_of_inputs] = m->wires[(*index)]->id;
        }
        else
        {
            m->wires[(*index)]->num_of_outputs = 0; // gate's output
            m->nodes[m->nodecount - 1]->output_wire_id = m->wires[(*index)]->id;
        }

        m->wires[(*index)]->name = strdup(token);

        set_wire_io(m, m->wires[(*index)], token, in_out_flag);
        *index = (*index) + 1;
    }
    else
    {
        if (!in_out_flag)
        {
            BackUp_w->num_of_outputs++;
            if (BackUp_w->num_of_outputs > initial_output_ids_size)
            {
                initial_output_ids_size++;
                BackUp_w->output_ids = (int *)realloc(BackUp_w->output_ids, initial_output_ids_size); //! PROSOXH
            }
            m->nodes[m->nodecount - 1]->input_wires_ids[m->nodes[m->nodecount - 1]->num_of_inputs] = BackUp_w->id;
        }
        else
        {
            m->nodes[m->nodecount - 1]->output_wire_id = BackUp_w->id;
        }

        set_wire_io(m, BackUp_w, token, in_out_flag);
    }
}

///////////////////NODES////////////////////////
void setNode(node *n, char *name, char *type, int node_id)
{
    strcpy(n->gate_name, name);
    strcpy(n->type_gate, type);
    n->id = node_id; /*Store node id*/
}
