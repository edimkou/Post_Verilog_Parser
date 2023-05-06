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

/**
 * Parses a verilog file and create a circuit object using the module
 * @param the circuit object, the file name
 */
void parse_verilog_file(module *m, char *filename)
{
    FILE *verilog;                  /* Verilog file */
    verilog = fopen(filename, "r"); /* Open Verilog file */
    if (!verilog)
    {
        fprintf(stderr, "transfer:  cannot open file \"%s\"\n", filename);
        exit(1);
    }

    // module m = (module)calloc(1, sizeof(struct module_)); /*Declare an instance of a module*/
    build_module(verilog, m);         /*Create module object*/
    rewind(verilog);                  /*Sets the stream position indicator to the beginning of verilog file.*/
    build_module_circuit(verilog, m); /*Create circuit object using the module*/

    fclose(verilog); /*Close the Verilog File*/
}

/**
 * Main program
 * @param argc, *argv[]
 */
int main(int argc, char *argv[])
{
    int i;
    if (argc != 2)
    {
        printf("Usage: ./verilog-parser <verilog_file.v>\n"); /* Check for a verilog input file */
        exit(1);
    }
    module *m = calloc(1, sizeof(struct module_)); /*Declare an instance of a circuit */

    m->name = strdup(argv[1]); /*Set circuit name*/

    parse_verilog_file(m, m->name); /*Parse the verilog file */

    print_circuit_summary(m); /*Print summary of the circuit */

    /*Free memory*/
    for (i = 0; i < m->outputcount; i++)
        free(m->output_names[i]);

    for (i = 0; i < m->inputcount; i++)
    {
        free(m->input_names[i]);
    }
    free(m->input_values);

    for (i = 0; i < m->nodecount; i++)
        free(m->nodes[i]);
    free(m->nodes);

    for (i = 0; i < m->gatecount; i++)
        free(m->gates[i]);

    for (i = 0; i < m->wirecount; i++)
    {
        free(m->wires[i]->name);
        free(m->wires[i]->type);
        free(m->wires[i]->output_ids);
    }
    free(m->wires);

    for (i = 0; i < m->regcount; i++)
    {
        free(m->regs[i]);
    }

    free(m->name);
    free(m); /*Deallocate memory used by the circuit*/

    return 0;
}