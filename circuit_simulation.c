#include "functions.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double get_random() { return ((double)rand() / (double)RAND_MAX); }

/**
 * Evaluates a gate
 * @param the name's operation, inputs, output
 */
void evaluate_operation(char *operation, bool elements[5], bool *res) {
  if (!strcmp(operation, "AND2")) {
    *res = (elements[0] & elements[1]);
  } else if (!strcmp(operation, "AND3")) {
    *res = (elements[0] & elements[1] & elements[2]);
  } else if (!strcmp(operation, "AND4")) {
    *res = (elements[0] & elements[1] & elements[2] & elements[3]);
  } else if (!strcmp(operation, "AND5")) {
    *res =
        (elements[0] & elements[1] & elements[2] & elements[3] & elements[4]);
  } else if (!strcmp(operation, "NAND2")) {
    *res = !(elements[0] & elements[1]);
  } else if (!strcmp(operation, "NAND3")) {
    *res = !(elements[0] & elements[1] & elements[2]);
  } else if (!strcmp(operation, "NAND4")) {
    *res = !(elements[0] & elements[1] & elements[2] & elements[3]);
  } else if (!strcmp(operation, "NAND5")) {
    *res =
        !(elements[0] & elements[1] & elements[2] & elements[3] & elements[4]);
  }
  if (!strcmp(operation, "OR2")) {
    *res = (elements[0] | elements[1]);
  } else if (!strcmp(operation, "OR3")) {
    *res = (elements[0] | elements[1] | elements[2]);
  } else if (!strcmp(operation, "OR4")) {
    *res = (elements[0] | elements[1] | elements[2] | elements[3]);
  } else if (!strcmp(operation, "OR5")) {
    *res =
        (elements[0] | elements[1] | elements[2] | elements[3] | elements[4]);
  }
  if (!strcmp(operation, "NOR2")) {
    *res = !(elements[0] | elements[1]);
  } else if (!strcmp(operation, "NOR3")) {
    *res = !(elements[0] | elements[1] | elements[2]);
  } else if (!strcmp(operation, "NOR4")) {
    *res = !(elements[0] | elements[1] | elements[2] | elements[3]);
  } else if (!strcmp(operation, "NOR5")) {
    *res =
        !(elements[0] | elements[1] | elements[2] | elements[3] | elements[4]);
  } else if (!strcmp(operation, "NOT") || !strcmp(operation, "INV")) {
    *res = !elements[0];
  }
}

/**
 * Sets the values of the inpout wires
 * @param the module object
 */
void primary_inputs_initialization(module *m) {
  int i = 0;
  double n = 0;

  for (i = 0; i < m->inputcount; i++) {
    n = 0;
    n = get_random(); // call the function to get a different value of n every
                      // time
    if (n - 0.49 > 0) {
      m->input_values[i] = 1;
    } else {
      m->input_values[i] = 0;
    }
  }
}

/**
 * Startes from a output and goes backward until it finds
 * the gate that have as inputs, module's inputs
 * @param the module object, a module's output wire, a SET flag(strikes),
 * the id of the gate whose value we will change
 * @return gate's result
 */
bool calculate(module *m, wire *w, const bool strikes, const int rand_set) {
  int i = 0, j = 0, k = 0, l = 0, next = 0;
  bool results[5], gate_result = 0;

  for (i = 0; i < m->nodecount - 1; i++) {

    if (m->nodes[i]->id ==
        w->input_id) // Searches all the gates that have as ouput this wire,
                     // this is based on the wire's id.
    {

      for (j = 0; j < m->nodes[i]->num_of_inputs;
           j++) // Goes to all the gate's input wires.
      {
        for (k = 0; k < m->wirecount; k++) {

          if (m->nodes[i]->input_wires_ids[j] == m->wires[k]->id) {
            // If the wire is a primary input then the program stores its
            // value on result array else recalls the func and goes to the
            // inputs of the gates that is as ouptput
            if (!m->wires[k]->input_id) {
              for (l = 0; l < m->inputcount; l++) {
                if (strcmp(m->wires[k]->name, m->input_names[l]) == 0) {
                  results[next] = m->input_values[l];
                  next++;
                }
              }
            } else {
              results[next] = calculate(m, m->wires[k], strikes, rand_set);
              next++;
            }
            break;
          }
        }
      }

      evaluate_operation(m->nodes[i]->type_gate, results, &gate_result);

      if (strikes && rand_set == m->nodes[i]->id) {
        gate_result = !gate_result;
      }
      m->nodes[i]->node_value = gate_result;
    }
  }
  return gate_result;
}

/**
 * Calls "calculate" with the given output wire as a starting point
 * @param the module object, module's output wire, a SET flag, num of SETS
 * @return output's wire value
 */
bool solve(module *m, wire *output_wire, bool strikes,
           int *num_of_hits) // circuit's output wire
{
  int rand_set = 0;
  if (strikes) {
    rand_set =
        rand() % (m->gatecount) + 1; // rand_set will not be equal to 0 because
    // the node ids = 0 is the primary inputs
    if (output_wire->input_id == rand_set) { // the gate that generates the
                                             // primary output can not be hitten
      strikes = 0;
      return calculate(m, output_wire, strikes, rand_set);
    }
    (*num_of_hits)++;
  }

  return calculate(m, output_wire, strikes, rand_set);
}

/**
 * Solves the circuit from a verilog module object
 * @param the module object
 */
void solving_circuit(module *m, int summary_flag) {
  int i = 0, j = 0, k = 0;

  srand(time(NULL)); // randomize seed

  m->output_values = (bool *)calloc(m->outputcount, sizeof(bool));
  int *num_of_set = (int *)calloc(m->outputcount, sizeof(int));

  if (summary_flag) {
    primary_inputs_initialization(m); // set the primary input values randomly
    for (k = 0; k < m->inputcount; k++) {
      printf("%s: %d\n", m->input_names[k], m->input_values[k]);
    }

    for (i = 0; i < m->outputcount; i++) {
      for (j = 0; j < m->wirecount; j++) {
        if (!strcmp(m->wires[j]->name, m->output_names[i])) {
          m->output_values[i] = solve(m, m->wires[j], 0, &num_of_set[i]);
          break;
        }
      }
    }
    print_circuit_summary(m); /*Print summary of the circuit */

  } else {
    bool *output_values_with_set = (bool *)calloc(m->outputcount, sizeof(bool));
    int *ser_array =
        (int *)calloc(m->outputcount,
                      sizeof(int)); // counter of ser for each output

    for (int repetition = 0; repetition < 1000;
         repetition++) {                // solves the circuit 1000 times
      primary_inputs_initialization(m); // set the primary input values randomly

      for (i = 0; i < m->outputcount; i++) {
        for (j = 0; j < m->wirecount; j++) {
          if (!strcmp(m->wires[j]->name, m->output_names[i])) {
            m->output_values[i] = solve(m, m->wires[j], 0, &num_of_set[i]);
            for (k = 0; k < 100;
                 k++) { // for each set of inputs the circuit receives 100
                        // Single Event Transient (SET) at a random gate
              output_values_with_set[i] =
                  solve(m, m->wires[j], 1, &num_of_set[i]);
              if (output_values_with_set[i] != m->output_values[i]) {
                ser_array[i]++;
              }
            }
            break;
          }
        }
      }
    }

    /*Printings the summary of the circuit after several particle strikes*/
    printf("\n\n******************LOGICAL MASKING******************\n");
    float ser_total = 0;
    for (i = 0; i < m->outputcount; i++) {
      printf("\nPrimary output: %s, Soft Errors: %d, Number of SET:%d, ",
             m->output_names[i], ser_array[i], num_of_set[i]);
      if (num_of_set[i] == 0) { // if num_of_set = 0, the division = infinity
        num_of_set[i] = 1;
      }
      printf("SER:%f%%.", ser_array[i] * 100.0 / num_of_set[i]);
      ser_total += ser_array[i] / (num_of_set[i] * 1.0);
    }
    printf("\n\nCircuit Soft Error Rate:%f%%.\n",
           ser_total * 100.0 / m->outputcount);
    printf("\n************************END************************\n");

    /* FREE THE MEMORY*/
    free(output_values_with_set);
    free(ser_array);
  }
  /* FREE THE MEMORY*/
  free(num_of_set);

  return;
}