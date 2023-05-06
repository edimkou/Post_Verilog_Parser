#ifdef IDENT_C
static const char *const<verilog_parser> _c_Id =
    "$Id$";
#endif

#include <stdio.h>
#include <string.h>
#include "functions.h"
/**
 * Prints the summary of a module - Statistical information
 * @param the module object
 */
void print_module_summary(module *m)
{
    int i = 0;
    printf("\n************** Module %s statistical results *************\n", m->name);
    printf("Number of inputs: %d\n", m->inputcount);
    for (i = 0; i < m->inputcount; i++)
        printf("%s ", m->input_names[i]);

    printf("\n\nNumber of outputs: %d\n", m->outputcount);
    for (i = 0; i < m->outputcount; i++)
        printf("%s ", m->output_names[i]);

    printf("\n\nNumber of gates: %d\n", m->gatecount);
    for (i = 0; i < m->gatecount; i++)
        printf("%s ", m->gates[i]);

    printf("\n\nNumber of wires: %d\n", m->wirecount);
    for (i = 0; i < m->wirecount; i++)
        printf("%s ", m->wires[i]->name);

    printf("\n\nNumber of regs: %d\n", m->regcount);
    for (i = 0; i < m->regcount; i++)
        printf("%s ", m->regs[i]);
    printf("*************************** END **************************\n\n");
}

/**
 * Prints the summary of a circuit - Statistical information
 * @param the circuit object
 */
void print_circuit_summary(module *m)
{
    int i = 0, j = 0;
    printf("\n************** Circuit %s statistical results *************\n", m->name);
    printf("Number of primary inputs: %d\n", m->inputcount);
    for (i = 0; i < m->inputcount; i++)
        printf("%s ", m->input_names[i]);

    printf("\n\nNumber of outputs: %d\n", m->outputcount);
    for (i = 0; i < m->outputcount; i++)
        printf("%s ", m->output_names[i]);

    printf("\n\nNumber of gates: %d\n", m->gatecount);
    for (i = 0; i < m->gatecount; i++)
    {
        printf("%s ", m->nodes[i]->gate_name);
    }

    printf("\n");

    printf("\n********************* Gates and their wires *********************\n");
    while (j < m->nodecount - 1)
    {
        printf("\n\nGate: %s, Type: %s, ID : %d, nums_of_input = %d\nInput wires id:", m->nodes[j]->gate_name, m->nodes[j]->type_gate, m->nodes[j]->id, m->nodes[j]->num_of_inputs);
        for (i = 0; i < m->nodes[j]->num_of_inputs; i++)
        {
            printf(" %d,", m->nodes[j]->input_wires_ids[i]);
        }
        printf("\nOutput wire id : %d", m->nodes[j]->output_wire_id);
        j++;
    }
    printf("\n");

    printf("\n********************* Wires and their connection with gates *********************\n");
    for (j = 0; j < m->wirecount; j++)
    {
        printf("\nWire: name %s ID: %d, id_gate Out %d\n", m->wires[j]->name, m->wires[j]->id, m->wires[j]->input_id);
        printf("the num of the gates the wire drives, %d, id_gate In:", m->wires[j]->num_of_outputs);

        for (i = 0; i < m->wires[j]->num_of_outputs; i++)
        {
            printf(" %d,", m->wires[j]->output_ids[i]);
        }

        if (m->wires[j]->num_of_outputs == 0) // It's a module output
        {
            printf(" %d,", m->wires[j]->output_ids[0]);
        }
        printf("\n");
    }

    printf("\n********************* Circuit Evaluation *********************\n");

    solving_circuit(m);
    printf("\n*************************** END **************************\n");
}
