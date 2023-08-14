#ifndef MODULE_FUNC_H
#define MODULE_FUNC_H

#include <stdbool.h>

#define TOKENSIZE 3104 /* Maximum length of a token.*/
#define LINESIZE 1024  /* Maximum length of each input line read.*/
#define BUFSIZE 2048   /* Maximum length of a buffer.*/

struct wire_ {
  int id;          /*Wire ID number*/
  char *name;      /*Name of this wire*/
  bool value;      /*Value of wire*/
  int input_id;    /*the id of the input gate-node*/
  int *output_ids; /*the ids of the gates that are next*/
  int num_of_outputs;
  int num_of_outputs_old;
};
typedef struct wire_ wire;

struct node_ {
  char gate_name[4]; /* name of the gate */
  char type_gate[5];
  int num_of_inputs;
  int id; /* node number */
  int node_value;
  int output_wire_id;
  int input_wires_ids[5]; /* array of input wires and I assume that the maximun
                           num of inputs in a gate is 5 */
};
typedef struct node_ node;

struct module_ {
  wire **wires;                /*Array of all wires */
  node **nodes;                /*Array of nodes*/
  char *name;                  /*Name of the circuit. */
  int inputcount, outputcount; /*Count of primary inputs and primary outputs. */
  int gatecount, wirecount, nodecount,
      regcount; /*Number of gates, wires, nodes, regs*/
  char *output_names[TOKENSIZE + 1],
      *input_names[TOKENSIZE + 1]; /*List of outputs in the netlist*/
  char *gates[TOKENSIZE + 1], *regs[TOKENSIZE + 1];
  bool *input_values, *output_values;
};
typedef struct module_ module;

#endif