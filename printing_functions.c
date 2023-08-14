#include "module.h"
#include <stdio.h>
#include <string.h>

/**
 * Prints the summary of a module - Statistical information
 * @param the module object
 */
void print_module_summary(module *m) {
  int i = 0;
  printf("\n************** Module %s statistical results *************\n",
         m->name);
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
void print_circuit_summary(module *m) {
  int i = 0, j = 0;

  // m->wires[j]->input_id is the id of the gate that is ganerated, if it is = 0
  // then is a primary input. m->wires[j]->output_ids is a array with the ids of
  // the gates that is drives, if it is = 0 this wire does not drive any gate.
  printf("\n************** Wires and their connection with gates "
         "**************\n");
  for (j = 0; j < m->wirecount; j++) {
    printf("\nWire: name %s ID: %d, id of the gate that is ganerated: %d\n",
           m->wires[j]->name, m->wires[j]->id, m->wires[j]->input_id);
    printf("the num of the gates the wire drives %d, their ids:",
           m->wires[j]->num_of_outputs);

    for (i = 0; i < m->wires[j]->num_of_outputs; i++) {
      printf(" %d,", m->wires[j]->output_ids[i]);
    }

    if (m->wires[j]->num_of_outputs == 0) {
      printf(" %d,", m->wires[j]->output_ids[0]);
    }
    printf("\n");
  }

  j = 0;
  printf("\n\n********************* Gates and their wires "
         "*********************\n");
  while (j < m->nodecount - 1) {
    printf(
        "\nGate: %s, Type: %s, ID : %d, nums_of_input = %d\nInput's wires id:",
        m->nodes[j]->gate_name, m->nodes[j]->type_gate, m->nodes[j]->id,
        m->nodes[j]->num_of_inputs);
    for (i = 0; i < m->nodes[j]->num_of_inputs; i++) {
      printf(" %d,", m->nodes[j]->input_wires_ids[i]);
    }
    printf("  --->  Output's wire id: %d", m->nodes[j]->output_wire_id);
    printf(" ---> Value: %d\n", m->nodes[j]->node_value);
    j++;
  }

  for (i = 0; i < m->outputcount; i++) {
    printf("\nThe Module's Output wire:%s is equal to: %d.", m->output_names[i],
           m->output_values[i]);
  }
  printf("\n*************************** END **************************\n");
}
