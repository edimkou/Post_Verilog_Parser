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
/**
 * Parses a verilog file and extract modules statistical data
 * @param the name of the verilog file, the module object
 */
void build_module(FILE *verilog, module *m)
{
    int i = 0, j = 0;                                          /*Indexes*/
    char linebuf[LINESIZE], tmpbuf[LINESIZE], buffer[BUFSIZE]; /*Buffer for lines of the verilog code*/
    char *token[TOKENSIZE];                                    /*Array to hold tokens for the line*/
    char *keyword;                                             /*Keyword from verilog line*/
    m->gatecount = 0;
    m->wires = (wire **)calloc((10), sizeof(wire *));
    int size = 10;

    /*Make first pass to count primary inputs, primary outputs, and wires.*/
    while (fgets(linebuf, LINESIZE, verilog) != NULL)
    {
        i = 0;
        strcpy(buffer, ""); /*Clear the buffer*/
        strcpy(tmpbuf, linebuf);
        token[0] = strtok(tmpbuf, " [()],;"); /*Get 1st keyword from the line*/
        keyword = trim(token[0]);             // first word of the line

        if (!reserved(keyword))
            continue; /*skip comment lines, empty lines or spaces*/

        strcat(buffer, linebuf);
        while (!end_of_line(linebuf))
        { /*Check if the line ends with a ';' character (Multiple lines statement)*/
            if (end_of_module(linebuf))
                break;                                     /*If end of module*/
            if (fgets(linebuf, LINESIZE, verilog) != NULL) /*Otherwise, append all the following lines*/
                strcat(buffer, linebuf);
        }

        token[0] = strtok(buffer, " [()],;"); /*Tokenize the line to extract data*/
        while (token[i] != NULL)
        {
            i++;
            token[i] = strtok(NULL, " [()],;\r\n");
        }

        if (strcmp(keyword, "module") == 0)
        {                                                         /*MODULES*/
            m->name = calloc(strlen(token[1]) + 1, sizeof(char)); /*Allocating memory for module name string*/
            strcpy(m->name, token[1]);                            /*Set module name*/
        }

        else if (strcmp(keyword, "input") == 0)
        { /*INPUTS*/
            for (j = 1; j < i; j++)
            {
                if (strcmp(token[j], "CK") && strcmp(token[j], "VDD") && strcmp(token[j], "GND"))
                {
                    /*Parse all the words in the line*/
                    if (signal_vector(token[j])) /*Handle a vector of signals*/
                        parse_signal_vector(m->input_names, token, &j, &m->inputcount);
                    else
                    {
                        // m->inputs = (m_input **)calloc(1, sizeof(m_input *)); /*Not a vector of signal*/
                        m->input_names[m->inputcount] = calloc(strlen(token[j]) + 1, sizeof(char)); /*Allocating memory for input string*/
                        strcpy(m->input_names[m->inputcount], token[j]);                            /*Add the input name to the array of inputs*/
                        m->inputcount++;                                                            /*Update the number of inputs in the circuit*/
                    }
                }
            }
        }

        else if (strcmp(keyword, "output") == 0)
        { /*OUTPUTS*/
            for (j = 1; j < i; j++)
            {                                /*Parse all the words in the line*/
                if (signal_vector(token[j])) /*Handle a vector of signals*/
                    parse_signal_vector(m->output_names, token, &j, &m->outputcount);
                else
                {                                                                                 /*Not a vector of signal*/
                    m->output_names[m->outputcount] = calloc(strlen(token[j]) + 1, sizeof(char)); /*Allocating memory for output string*/
                    strcpy(m->output_names[m->outputcount], token[j]);                            /*Add the output name to the array of outputs*/
                    m->outputcount++;                                                             /*Update the number of outputs in the circuit*/
                }
            }
        }

        else if (strcmp(keyword, "wire") == 0)
        { /*WIRES*/
            for (j = 1; j < i; j++)
            {                                /*Parse all the words in the line*/
                if (signal_vector(token[j])) /*Handle a vector of signals*/
                {
                    // parse_signal_vector(m->wires, token, &j, &m->wirecount);
                    strncpy(token[j], token[j], strlen(token[j]) - 1);
                    m->wires[m->wirecount]->name = strdup(token[j]);
                }
                else
                {
                    m->wires[m->wirecount] = (wire *)calloc(1, sizeof(struct wire_));
                    m->wires[m->wirecount]->output_ids = (int *)calloc(1, sizeof(int)); // initial size is 5 and if we need more we'll allocate later
                    /*Not a vector of signal*/
                    // m->wires[m->wirecount] = calloc(strlen(token[j]) + 1, sizeof(char)); /*Allocating memory for wire string*/
                    // strcpy(m->wires[m->wirecount]->name, token[j]); /*Add the wire name to the array of wires*/
                    m->wires[m->wirecount]->name = strdup(token[j]);
                    m->wirecount++; /*Update the number of wires in the circuit*/
                    if (m->wirecount >= size)
                    {

                        size = size * 2;
                        m->wires = (wire **)realloc(m->wires, sizeof(wire *) * size);
                    }
                }
            }
        }

        else if (strcmp(keyword, "reg") == 0)
        { /*REGS*/
            for (j = 1; j < i; j++)
            {                                /*Parse all the words in the line*/
                if (signal_vector(token[j])) /*Handle a vector of signals*/
                    parse_signal_vector(m->regs, token, &j, &m->regcount);
                else
                {                                                                      /*Not a vector of signal*/
                    m->regs[m->regcount] = calloc(strlen(token[j]) + 1, sizeof(char)); /*Allocating memory for reg string*/
                    strcpy(m->regs[m->regcount], token[j]);                            /*Add the reg name to the array of regs*/
                    m->regcount++;                                                     /*Update the number of regs in the circuit*/
                }
            }
        }

        else if (gate(keyword))                                                  // here is starting the reading of the gates
        {                                                                        /*GATES*/
            m->gates[m->gatecount] = calloc(strlen(token[1]) + 1, sizeof(char)); /*Allocating memory for module name string*/
            strcpy(m->gates[m->gatecount], token[1]);                            /*Add the module name to the array of modules*/
            m->gatecount++;                                                      /*Update the number of modules in the circuit*/
        }

        else if (strstr(linebuf, "endmodule") != NULL) /*END OF MODULE*/
        {
            print_module_summary(m); /*Print summary of the module*/
            break;                   /*End of the module; break*/
        }
    } // end while

} /*end build_modules*/
