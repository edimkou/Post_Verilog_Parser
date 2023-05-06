

// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// functions.H
// A library of helper functions for the verilog parser
//
// Author: David Kebo Houngninou
// ---------------------------------------------------------------------------
#ifndef _functions_H
#define _functions_H

#ifdef IDENT_H
static const char *const functions_H_Id =
    "$Id$";
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define TOKENSIZE 128 /* Maximum length of a token.*/
#define LINESIZE 512  /* Maximum length of each input line read.*/
#define BUFSIZE 512   /* Maximum length of a buffer.*/

struct wire_
{
    int id;          /*Wire ID number*/
    char *name;      /*Name of this wire*/
    char *type;      /*Type of gate driving this wire*/
    bool value;      /*Value of wire*/
    bool primary;    /*Primary input flag*/
    int input_id;    /*the id of the input gate-node*/
    int *output_ids; /*the ids of the gates that are next*/
    int num_of_outputs;
};
typedef struct wire_ wire;

struct node_
{
    char gate_name[4]; // name of the gate
    char type_gate[5];
    int num_of_inputs;
    int id; // node number;
    int node_value;
    int output_wire_id;
    int input_wires_ids[5]; // I assume that the maximun num of inputes in a gate is 5 ///array of input wires
};
typedef struct node_ node;

struct module_
{
    wire **wires;                                                   /*Array of all wires */
    node **nodes;                                                   /*Array of nodes*/
    char *name;                                                     /*Name of the circuit. */
    int inputcount, outputcount;                                    /*Count of primary inputs and primary outputs. */
    int gatecount, wirecount, nodecount, regcount;                  /*Number of gates, wires, nodes, regs*/
    char *output_names[TOKENSIZE + 1], *input_names[TOKENSIZE + 1]; /*List of outputs in the netlist*/
    // m_input **inputs;                                              /*Array of all inputs and their values*/
    char *gates[TOKENSIZE + 1], *regs[TOKENSIZE + 1];
    bool *input_values;
};
typedef struct module_ module;

/*PROTOTYPES*/
void build_module(FILE *verilog, module *m);
void build_module_circuit(FILE *verilog, module *m);
void parse_signal_vector(char *signal_arr[], char *token[], int *index, int *count);
bool reserved(char *word);
bool gate(char *word);
char *trim(char *source);
bool signal_vector(char *word);
bool end_of_module(char *word);
bool end_of_line(char *source);
void print_module_summary(module *m);
void print_circuit_summary(module *m);
void setNode(node *n, char *name, char *type, int node_id);
void build_wire_(wire *BackUp_w, module *m, int *index, char *token, bool in_out_flag);
void set_wire_io(module *m, wire *w, char *name, bool in_out_flag);
bool defined_in(module *m, char *name);
bool defined_out(module *m, char *name);
wire *defined_wire(module *m, char *name);
bool dot_plus_char(char *source);
void solving_circuit(module *m);

/*END PROTOTYPES*/

#endif