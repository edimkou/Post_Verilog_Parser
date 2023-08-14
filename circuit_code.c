#include "functions.h"
#include "keywords.h"
#include "module.h"
#include <stdlib.h>
#include <string.h>

/**
 * Initialize the circuit from a verilog module object
 * @param the module object and a num
 */
void circuit_initialization(module *m, int *index) {
  /*Initialization of the circuit*/
  m->nodecount = 1;
  m->nodes = (node **)malloc(
      (m->gatecount) *
      sizeof(node *)); /*Allocate a contiguous array to index for every nodes*/
  m->input_values = (bool *)malloc(sizeof(bool) * m->inputcount);

  int w_size = m->inputcount + m->outputcount + m->wirecount + m->regcount;
  m->wires = (wire **)realloc(m->wires, sizeof(wire *) * w_size);
  for (int i = m->wirecount; i < w_size; i++) {
    m->wires[i] = NULL;
  }

  for (int i = 0; i < m->wirecount; i++) {
    m->wires[*index]->id = *index;
    m->wires[*index]->value = 0;
    m->wires[(*index)]->input_id = -1;
    m->wires[(*index)]->num_of_outputs = 0;
    m->wires[(*index)]->num_of_outputs_old = 1;
    (*index)++;
  }
}

/**
 * Build a circuit from a verilog module object
 * @param a FILE, the module object
 */
void build_module_circuit(FILE *verilog, module *m) {

  int i = 0, index = 0; /*Indexes*/
  bool in_out_flag = 0; // in = 0, out = 1;
  circuit_initialization(m, &index);
  char keyword[50] = {0}; /*Keyword from verilog line*/

  char linebuf[LINESIZE]; /*Buffer for lines of the verilog code*/
  char *buffer = NULL;
  char *token[TOKENSIZE]; /*Array to hold tokens for the line*/
  memset(linebuf, 0, sizeof(char) * LINESIZE);
  memset(token, 0, sizeof(char *) * TOKENSIZE);

  int buffer_size = LINESIZE;
  buffer = (char *)malloc(sizeof(char) * buffer_size);

  while (fgets(linebuf, LINESIZE, verilog) !=
         NULL) { /*Create wires: gate_type [gate_name](out_port,
                    in_port...)*/

    strcpy(buffer, ""); /*Clear the buffer*/

    strcpy(buffer, linebuf);
    strtok(buffer, " [()],;\n\r"); /*Get 1st keyword from the line*/
    strncpy(keyword, buffer, 50);  // first word of the line
    trim(keyword);

    i = 0;
    // New line
    if ((strstr(linebuf, "endmodule") != NULL))
      break; /*If end of module is reached then break*/
    if (!gate(keyword))
      continue; /*Skip everything but gates*/

    strcpy(buffer, linebuf);
    int bytes_read = LINESIZE;      /* The bytes have been read */
    while (!end_of_line(linebuf)) { /*Check if the line ends with a ';'
                                       character (Multiple lines statement)*/
      if (end_of_module(linebuf))
        break; /*If end of module*/
      strcpy(linebuf, "");
      if (fgets(linebuf, LINESIZE, verilog) !=
          NULL) { /*Otherwise, append all the following lines*/
        bytes_read += LINESIZE;
        if (bytes_read > buffer_size) {
          buffer_size = buffer_size * 2;
          buffer = (char *)realloc(buffer, sizeof(char) * buffer_size);
        }

        strcat(buffer, linebuf);
      }
    }

    // The first element is the name of the gate
    token[0] = strtok(buffer, " [()],;"); /*Tokenize the line to extract data*/

    while (token[i] != NULL) {
      i++;
      token[i] = strtok(NULL, " [()],;\r\n");
    }

    int j = 0;
    wire *BackUp_w;
    m->nodes[m->nodecount - 1] = (node *)malloc(sizeof(node));
    m->nodes[m->nodecount - 1]->num_of_inputs = 0;

    /*A. Create wires for all the gate inputs*/
    for (j = 2; j < i - 2; j++) // -2 and not -1 because j = i - 2 is .ZN
    {
      in_out_flag = 0; // it is a input
      if (!dot_plus_char(token[j])) {
        BackUp_w = defined_wire(m, token[j]);
        build_wire_(BackUp_w, m, &index, token[j], in_out_flag);
        (m->nodes[m->nodecount - 1]->num_of_inputs)++;
      }
    }

    /*C. Create wires for the gate output*/
    in_out_flag = 1; // its a output
    BackUp_w = defined_wire(m, token[i - 1]);
    build_wire_(BackUp_w, m, &index, token[i - 1], in_out_flag);

    // Node
    setNode(m->nodes[m->nodecount - 1], token[1], keyword,
            m->nodecount); /*Set values of a node (type, name, id)*/
    m->nodecount++;

    memset(token, 0, sizeof(char *) * TOKENSIZE);
    strcpy(linebuf, "");
    buffer_size = LINESIZE;
    buffer = (char *)realloc(buffer, sizeof(char) * buffer_size);
    memset(buffer, 0, sizeof(char) * buffer_size);
  } // end  while

  m->wirecount = index;
  free(buffer);
}
