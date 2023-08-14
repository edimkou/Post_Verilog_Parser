#include "functions.h"
#include "keywords.h"
#include <stdlib.h>
#include <string.h>

/**
 * Parses a string and removes all character after a numeric character
 * @param the source string
 */
char *trim(char *source) {
  int i = 0, index = 0;
  int sr_length = strlen(source);

  for (i = 0; i < sr_length; i++) {
    if ((source[i] == '\n' || source[i] == '_') && i != 0) {
      source[index] = '\0';
      break;
    } else if (source[i] == ' ') {
      continue;
    } else
      source[index++] = source[i];
  }
  source[index] = '\0';
  return source;
}

/**
 * Determines if a string is reserved keyword
 * @param the string to compare
 * @return whether the string is reserved or not
 */
bool reserved(char *word) {
  int i;
  for (i = 0; i < RESERVEDNUM; i++) {
    if (strcmp(word, RESERVED_WORD[i]) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Determines if a string is gate
 * @param the string to check
 * @return whether the string is a gate or not
 */
bool gate(char *word) {
  int i;
  for (i = 0; i < GATESNUM; i++)
    if (strcmp(word, GATE_NAME[i]) == 0)
      return true;
  return false;
}

/**
 * Determines if end of module
 * @param the string to check
 * @return if end of module or not
 */
bool end_of_module(char *word) {
  if (strstr(word, "endmodule") != NULL)
    return true;
  return false;
}

/**
 * Parses a string and search for the character ';'
 * @param the source string
 * @return whether the character ';' is found or not
 */
bool end_of_line(char *source) {
  char *pchar = strchr(source, ';');
  return (pchar == NULL) ? false : true;
}

/**
 * Determines if a input is already created
 * @param the module object, the wire name
 * @return whether the wire is already created or not
 */
bool defined_in(module *m, char *name) {
  int i = 0;

  for (i = 0; i < m->inputcount; i++) {
    if (strcmp(m->input_names[i], name) == 0)
      return true;
  }
  return false;
}

/**
 * Determines if a output is already exists
 * @param the module object, the wire name
 * @return whether the wire exists or not
 */
bool defined_out(module *m, char *name) {
  int i = 0;
  for (i = 0; i < m->outputcount; i++) {
    if (strcmp(m->output_names[i], name) == 0)
      return true;
  }

  return false;
}

/**
 * Determines if a wire is already exists
 * @param the module object, the wire's name to check
 * @return whether the wire exists or not
 */
wire *defined_wire(module *m, char *name) {
  int i = 0;

  while (m->wires[i] != NULL) {
    if (strcmp(m->wires[i]->name, name) == 0)
      return m->wires[i];
    i++;
  }
  return NULL;
}

/**
 * Checks if the first char of a string is dot
 * @param the source string
 * @return whether the char is a dot or not
 */
bool dot_plus_char(char *source) {
  if (source[0] == '.') {
    return true;
  }

  return false;
}

/**
 * Sets the ids of the nodes that drives or be genereted
 * @param the module object, the wire, name to check and a
 * flag that specifies if this wire if a primary input-output
 */
void set_wire_io(module *m, wire *w, char *name, bool in_out_flag) {

  if (defined_in(m, name)) /*If wire is not already defined and is a input or
                              output of the module*/
  {
    w->input_id = 0;
    w->output_ids[w->num_of_outputs - 1] = m->nodecount;
    return;
  }

  if (defined_out(m, name)) /*If wire is not already defined and is a input or
                               output of the module*/
  {
    if (in_out_flag) {
      w->input_id = m->nodecount;
    } else {
      w->output_ids[w->num_of_outputs - 1] = m->nodecount;
    }
    return;
  }

  if (in_out_flag) // It's a wire
  {
    w->input_id = m->nodecount;
  } else {
    w->output_ids[w->num_of_outputs - 1] = m->nodecount;
  }
}

/**
 * Create a wire
 * @param the module object, the wire object, the wire name
 */
void build_wire_(wire *BackUp_w, module *m, int *index, char *token,
                 bool in_out_flag) {
  // int initial_output_ids_size = 1;
  if (BackUp_w == NULL) {
    m->wires[(*index)] = (wire *)malloc(sizeof(struct wire_));
    m->wires[(*index)]->output_ids =
        (int *)malloc(sizeof(int)); // the wire drives to 1  gate and if we
                                    // need more we'll allocate later
    m->wires[(*index)]->name = (char *)malloc(sizeof(char) * strlen(token));
    strcpy(m->wires[(*index)]->name, token);

    // initialization of the ids
    m->wires[(*index)]->input_id = -1; // the gate's id that the wire generated
    m->wires[(*index)]->output_ids[0] = -1;
    m->wires[(*index)]->id = (*index); // wires id

    if (!in_out_flag) {
      m->wires[(*index)]->num_of_outputs = 1;
      m->wires[(*index)]->num_of_outputs_old = 1;
      m->nodes[m->nodecount - 1]
          ->input_wires_ids[m->nodes[m->nodecount - 1]->num_of_inputs] =
          m->wires[(*index)]->id;
    } else {
      m->wires[(*index)]->num_of_outputs = 0; // gate's output
      m->wires[(*index)]->num_of_outputs_old = 0;
      m->nodes[m->nodecount - 1]->output_wire_id = m->wires[(*index)]->id;
    }

    set_wire_io(m, m->wires[(*index)], token, in_out_flag);
    *index = (*index) + 1;
  } else {
    if (!in_out_flag) { // input
      BackUp_w->num_of_outputs++;
      if (BackUp_w->num_of_outputs >= BackUp_w->num_of_outputs_old) {

        BackUp_w->num_of_outputs_old = BackUp_w->num_of_outputs;
        BackUp_w->output_ids =
            (int *)realloc(BackUp_w->output_ids,
                           sizeof(int) * BackUp_w->num_of_outputs); //! PROSOXH
      }
      m->nodes[m->nodecount - 1]
          ->input_wires_ids[m->nodes[m->nodecount - 1]->num_of_inputs] =
          BackUp_w->id;
    } else {
      m->nodes[m->nodecount - 1]->output_wire_id = BackUp_w->id;
    }

    set_wire_io(m, BackUp_w, token, in_out_flag);
  }
}

/**
 * Create a node
 * @param the node, the name, type, id
 */
void setNode(node *n, char *name, char *type, int node_id) {
  strcpy(n->gate_name, name);
  strcpy(n->type_gate, type);
  n->id = node_id;
  n->node_value = 0;
}
