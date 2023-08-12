

// ---------------------------------------------------------------------------
// functions.H
// A library of helper functions for the verilog parser
//
// ---------------------------------------------------------------------------
#ifndef _functions_H
#define _functions_H

#include "module.h"
#include <stdbool.h>
#include <stdio.h>

/*PROTOTYPES*/
void build_module_circuit(FILE *verilog, module *m);
void build_module(FILE *verilog, module *m);
bool reserved(char *word);
bool gate(char *word);
char *trim(char *source);
bool signal_vector(char *word);
bool end_of_line(char *source);
bool end_of_module(char *word);
void print_module_summary(module *m);
void print_circuit_summary(module *m);
void setNode(node *n, char *name, char *type, int node_id);
void build_wire_(wire *BackUp_w, module *m, int *index, char *token,
                 bool in_out_flag);
void set_wire_io(module *m, wire *w, char *name, bool in_out_flag);
bool defined_in(module *m, char *name);
bool defined_out(module *m, char *name);
wire *defined_wire(module *m, char *name);
bool dot_plus_char(char *source);
void solving_circuit(module *m, int summary_flag);
/*END PROTOTYPES*/

#endif