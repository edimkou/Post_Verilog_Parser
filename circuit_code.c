#ifdef IDENT_C
static const char *const<verilog_parser> _c_Id =
    "$Id$";
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "functions.h"
#include "keywords.h"

void circuit_initialization(module *m, int *index)
{
    int i = 0;
    /*Initialization of the circuit*/

    m->nodecount = 1;
    m->nodes = (node **)calloc((m->gatecount), sizeof(node *)); /*Allocate a contiguous array to index for every nodes*/
    m->input_values = (bool *)calloc(m->inputcount, sizeof(bool));

    for (i = 0; i < m->inputcount; i++)
    { /*Store the names of inputs*/
        printf(" Please enter 0 or 1 for %s input: ", m->input_names[i]);
        scanf(" %d", &(m->input_values[i]));
    }
    while (m->wires[*index] != NULL)
    {
        m->wires[*index]->id = *index;
        m->wires[*index]->value = 0;
        (*index)++;
    }
}

/**
 * Build a circuit from a verilog module object
 * @param a pointer to the verilog file, a verilog module object, a circuit object
 */
void build_module_circuit(FILE *verilog, module *m)
{

    int i = 0, index = 0; /*Indexes*/
    bool in_out_flag = 0; // in = 0, out = 1;
    circuit_initialization(m, &index);

    char linebuf[LINESIZE], tmpbuf[LINESIZE], buffer[BUFSIZE]; /*Buffer for lines of the verilog code*/
    char *token[TOKENSIZE];                                    /*Array to hold tokens for the line*/

    memset(linebuf, 0, sizeof(char) * LINESIZE);
    memset(tmpbuf, 0, sizeof(char) * LINESIZE);
    memset(buffer, 0, sizeof(char) * BUFSIZE);
    memset(token, 0, sizeof(char *) * TOKENSIZE);

    while (fgets(linebuf, LINESIZE - 1, verilog) != NULL)
    {                                              /*Create wires: gate_type [gate_name](out_port, in_port...)*/
        char *keyword = NULL;                      /*Keyword from verilog line*/
        memset(buffer, 0, sizeof(char) * BUFSIZE); /*Clear the buffer*/
        strcpy(tmpbuf, linebuf);
        token[0] = strtok(tmpbuf, " [()],;"); /*Get 1st keyword from the line*/
        keyword = trim(token[0]);

        i = 0;
        // New line
        if (!reserved(keyword))
            continue; /*Skip any comment lines, empty lines or spaces*/
        if ((strstr(linebuf, "endmodule") != NULL))
            break; /*If end of module is reached then break*/
        if (!gate(keyword))
            continue; /*Skip everything but gates*/

        strcat(buffer, linebuf);

        while (!end_of_line(linebuf))
        {                                                  /*Check if the line ends with a ';' character (Multiple lines statement)*/
            if (fgets(linebuf, LINESIZE, verilog) != NULL) /*Otherwise, append all the following lines until ';' is found*/
                strcat(buffer, linebuf);
        }

        // The first element is the name of the gate
        token[0] = strtok(buffer, " [()],;"); /*Tokenize the line to extract data*/
        while (token[i] != NULL)
        {
            i++;
            token[i] = strtok(NULL, " [()],;\r\n");
        }

        int j = 0;
        wire *BackUp_w;
        m->nodes[m->nodecount - 1] = (node *)calloc(1, sizeof(node));
        m->nodes[m->nodecount - 1]->num_of_inputs = 0;

        /*A. Create wires for all the gate inputs*/
        for (j = 2; j < i - 2; j++) // -2 and not -1 because j = i - 2 is .ZN
        {
            in_out_flag = 0; // it is a input
            if (!dot_plus_char(token[j]))
            {
                BackUp_w = defined_wire(m, token[j]);
                build_wire_(BackUp_w, m, &index, token[j], in_out_flag);
                (m->nodes[m->nodecount - 1]->num_of_inputs)++;
            }
        }

        /*C. Create wires for the gate output*/
        in_out_flag = 1; // its a output
        BackUp_w = defined_wire(m, token[i - 1]);
        build_wire_(BackUp_w, m, &index, token[i - 1], in_out_flag);

        // Node
        setNode(m->nodes[m->nodecount - 1], token[1], keyword, m->nodecount); /*Set values of a node (type, name, id)*/
        m->nodecount++;

        memset(linebuf, 0, sizeof(char) * LINESIZE);
        memset(tmpbuf, 0, sizeof(char) * LINESIZE);
        memset(buffer, 0, sizeof(char) * BUFSIZE);
        memset(token, 0, sizeof(char *) * TOKENSIZE);
    } // end  while
    m->wirecount = index;
}
