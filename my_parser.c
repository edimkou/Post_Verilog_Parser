#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Parses a verilog file and create a circuit object using the module
 * @param the circuit object, the file name
 */
void parse_verilog_file(module *m, char *filename) {
  FILE *verilog;                  /* Verilog file */
  verilog = fopen(filename, "r"); /* Open Verilog file */
  if (!verilog) {
    fprintf(stderr, "transfer:  cannot open file \"%s\"\n", filename);
    exit(1);
  }

  // of a module*/
  build_module(verilog, m); /*Create module object*/
  rewind(verilog); /*Sets the stream position indicator to the beginning of
                      verilog file.*/
  build_module_circuit(verilog, m); /*Create circuit object using the module*/
  fclose(verilog);                  /*Close the Verilog File*/
}

/**
 * Main program
 * @param argc, *argv[]
 */
int main(int argc, char *argv[]) {
  int i;
  if (argc != 2) {
    printf("Usage: ./verilog-parser <verilog_file.v>\n"); /* Check for a verilog
                                                             input file */
    exit(1);
  }
  module *m =
      calloc(1, sizeof(struct module_)); /*Declare an instance of a circuit */

  m->name = strdup(argv[1]); /*Set circuit name*/

  parse_verilog_file(m, m->name); /*Parse the verilog file */
  printf("Printing choices: \nEnter 1 for the SUMMARY of the circuit "
         "for a set of random "
         "inputs \n OR \nEnter 0 for the LOGICAL "
         "MASKING of the circuit: ");
  int summary_flag = -1;
  scanf("%d", &summary_flag);
  solving_circuit(m, summary_flag);
  // print_circuit_summary(m); /*Print summary of the circuit */

  //***************Free The Memory****************//
  for (i = 0; i < m->nodecount - 1; i++)
    if (m->nodes[i] != NULL) {
      free(m->nodes[i]);
      m->nodes[i] = NULL;
    }
  free(m->nodes);

  for (i = 0; i < m->outputcount; i++) {
    if (m->output_names[i] != NULL) {
      free(m->output_names[i]);
      m->output_names[i] = NULL;
    }
  }
  free(m->output_values);

  for (i = 0; i < m->inputcount; i++) {
    if (m->input_names[i] != NULL) {
      free(m->input_names[i]);
      m->input_names[i] = NULL;
    }
  }
  free(m->input_values);

  for (i = 0; i < m->gatecount; i++) {
    if (m->gates[i] != NULL) {
      free(m->gates[i]);
      m->gates[i] = NULL;
    }
  }

  for (i = 0; i < m->wirecount; i++) {
    if (m->wires[i]->name != NULL) {
      free(m->wires[i]->name);
      m->wires[i]->name = NULL;
    }
    for (int j = 0; j < m->wires[i]->num_of_outputs; ++j) {
      if (m->wires[i]->output_ids != NULL) {
        free(m->wires[i]->output_ids);
        m->wires[i]->output_ids = NULL;
      }
    }
  }
  i = 0;
  while (m->wires[i] != NULL) {
    free(m->wires[i]);
    m->wires[i] = NULL;
    i++;
  }

  free(m->wires);

  for (i = 0; i < m->regcount; i++) {
    if (m->regs[i] != NULL) {
      free(m->regs[i]);
      m->regs[i] = NULL;
    }
  }

  free(m->name);
  free(m); /*Deallocate memory used by the circuit*/

  return 0;
}
