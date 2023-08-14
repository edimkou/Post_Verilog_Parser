#include "functions.h"
#include "keywords.h"
#include "module.h"
#include <stdlib.h>
#include <string.h>

/**
 * Parses a verilog file and extract modules statistical data
 * @param the name of the verilog file, the module object
 */
void build_module(FILE *verilog, module *m) {
  int i = 0, j = 0;       /*Indexes*/
  char linebuf[LINESIZE]; /*Buffer for lines of the verilog code*/
  char *buffer = {0};
  char *token[TOKENSIZE]; /*Array to hold tokens for the line*/
  char keyword[50] = {0}; /*Keyword from verilog line*/
  m->gatecount = 0;
  m->wires = (wire **)malloc(sizeof(wire *) * 5);
  int size = 5;

  int buffer_size = LINESIZE;
  buffer = (char *)malloc(sizeof(char) * buffer_size);

  /*Make first pass to count primary inputs, primary outputs,  wires and
   * gates.*/
  while (fgets(linebuf, LINESIZE, verilog) != NULL) {
    i = 0;

    strcpy(buffer, linebuf);
    strtok(buffer, " [()],;\n\r"); /*Get 1st keyword from the line*/
    strncpy(keyword, buffer, 50);  // first word of the line
    trim(keyword);

    if (!reserved(keyword))
      continue; /*skip comment lines, empty lines or spaces*/

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

    /*Tokenize the line to extract data*/
    i = 1;
    token[0] = strtok(buffer, " [()],;");
    while ((token[i] = strtok(NULL, " [()],;\r\n"))) {
      ++i;
    }

    if (strcmp(keyword, "module") == 0) { /*MODULES*/
      m->name =
          malloc(sizeof(char) *
                 strlen(token[1])); /*Allocating memory for module name string*/
      strcpy(m->name, token[1]);    /*Set module name*/
    }

    else if (strcmp(keyword, "input") == 0) { /*INPUTS*/
      for (j = 1; j < i; j++) {
        if (strcmp(token[j], "CK") && strcmp(token[j], "VDD") &&
            strcmp(token[j], "GND")) { /*Parse all the words in the line*/
          m->input_names[m->inputcount] =
              malloc(sizeof(char) *
                     strlen(token[j])); /*Allocating memory for input string*/
          strcpy(m->input_names[m->inputcount],
                 token[j]); /*Add the input name to the array of inputs*/
          m->inputcount++;  /*Update the number of inputs in the circuit*/
        }
      }
    }

    else if (strcmp(keyword, "output") == 0) { /*OUTPUTS*/
      for (j = 1; j < i; j++) { /*Parse all the words in the line*/
        m->output_names[m->outputcount] = (char *)malloc(
            sizeof(char) *
            strlen(token[j])); /*Allocating memory for output string*/
        strcpy(m->output_names[m->outputcount],
               token[j]); /*Add the output name to the array of outputs*/
        m->outputcount++; /*Update the number of outputs in the circuit*/
      }
    }

    else if (strcmp(keyword, "wire") == 0) { /*WIRES*/
      for (j = 1; j < i; j++) {              /*Parse all the words in the line*/
        if (token[j] != NULL) {
          m->wires[m->wirecount] = (wire *)malloc(sizeof(struct wire_));
          m->wires[m->wirecount]->output_ids =
              (int *)malloc(sizeof(int)); // initial size is 1 and if we need
                                          // more we'll allocate later
          m->wires[m->wirecount]->output_ids[0] = -1;

          m->wires[m->wirecount]->name =
              (char *)malloc(sizeof(char) * strlen(token[j]));
          strcpy(m->wires[m->wirecount]->name, token[j]);

          m->wirecount++; /*Update the number of wires in the circuit*/
          if (m->wirecount >= size) {
            size = size * 2;
            m->wires = (wire **)realloc(m->wires, sizeof(wire *) * size);
          }
        }
      }
    }

    else if (strcmp(keyword, "reg") == 0) { /*REGS*/
      for (j = 1; j < i; j++) {             /*Parse all the words in the line*/
        m->regs[m->regcount] =
            malloc(sizeof(char) *
                   strlen(token[j])); /*Allocating memory for reg string*/
        strcpy(m->regs[m->regcount],
               token[j]); /*Add the reg name to the array of regs*/
        m->regcount++;    /*Update the number of regs in the circuit*/
      }
    }

    else if (gate(keyword)) // here is starting the reading of the gates
    {                       /*GATES*/
      m->gates[m->gatecount] =
          malloc(sizeof(char) *
                 strlen(token[1])); /*Allocating memory for module name string*/
      strcpy(m->gates[m->gatecount],
             token[1]); /*Add the module name to the array of modules*/
      m->gatecount++;   /*Update the number of modules in the circuit*/
    }

    else if (strstr(linebuf, "endmodule") != NULL) /*END OF MODULE*/
    {
      print_module_summary(m); /*Print summary of the module*/
      break;                   /*End of the module; break*/
    }

    buffer_size = LINESIZE;
    buffer = (char *)realloc(buffer, sizeof(char) * buffer_size);
    strcpy(buffer, ""); /*Clear the buffer*/
  }                     // end while
  free(buffer);

} /*end build_modules*/
