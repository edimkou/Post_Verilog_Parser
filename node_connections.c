#include <stdio.h>
#include <string.h>
#include "functions.h"

void evaluate_operation(char *operation, bool elements[5], bool *res)
{
    if (!strcmp(operation, "AND2"))
    {
        *res = (elements[0] & elements[1]);
    }
    else if (!strcmp(operation, "AND3"))
    {
        *res = (elements[0] & elements[1] & elements[2]);
    }
    else if (!strcmp(operation, "AND4"))
    {
        *res = (elements[0] & elements[1] & elements[2] & elements[3]);
    }
    else if (!strcmp(operation, "AND5"))
    {
        *res = (elements[0] & elements[1] & elements[2] & elements[3] & elements[4]);
    }
    else if (!strcmp(operation, "NAND2"))
    {
        *res = !(elements[0] & elements[1]);
    }
    else if (!strcmp(operation, "NAND3"))
    {
        *res = !(elements[0] & elements[1] & elements[2]);
    }
    else if (!strcmp(operation, "NAND4"))
    {
        *res = !(elements[0] & elements[1] & elements[2] & elements[3]);
    }
    else if (!strcmp(operation, "NAND5"))
    {
        *res = !(elements[0] & elements[1] & elements[2] & elements[3] & elements[4]);
    }
    if (!strcmp(operation, "OR2"))
    {
        *res = (elements[0] | elements[1]);
    }
    else if (!strcmp(operation, "OR3"))
    {
        *res = (elements[0] | elements[1] | elements[2]);
    }
    else if (!strcmp(operation, "OR4"))
    {
        *res = (elements[0] | elements[1] | elements[2] | elements[3]);
    }
    else if (!strcmp(operation, "OR5"))
    {
        *res = (elements[0] | elements[1] | elements[2] | elements[3] | elements[4]);
    }
    if (!strcmp(operation, "NOR2"))
    {
        *res = !(elements[0] | elements[1]);
    }
    else if (!strcmp(operation, "NOR3"))
    {
        *res = !(elements[0] | elements[1] | elements[2]);
    }
    else if (!strcmp(operation, "NOR4"))
    {
        *res = !(elements[0] | elements[1] | elements[2] | elements[3]);
    }
    else if (!strcmp(operation, "NOR5"))
    {
        *res = !(elements[0] | elements[1] | elements[2] | elements[3] | elements[4]);
    }
    else if (!strcmp(operation, "NOT") || !strcmp(operation, "INV"))
    {
        *res = !elements[0];
    }
}

// This function is starting from a output and goes backward until it finds the gate that have as inputs modules inputs.
bool calculate(module *m, wire *w)
{
    int i = 0, j = 0, k = 0, l = 0, next = 0;
    bool results[5], gate_result = 0;
    char op[5];

    for (i = 0; i < m->nodecount - 1; i++)
    {

        if (m->nodes[i]->id == w->input_id)
        {
            strcpy(op, m->nodes[i]->type_gate);
            for (j = 0; j < m->nodes[i]->num_of_inputs; j++)
            {
                for (k = 0; k < m->wirecount; k++)
                {

                    if (m->nodes[i]->input_wires_ids[j] == m->wires[k]->id)
                    {

                        if (m->wires[k]->input_id == 0)
                        {
                            printf("\n\nGate's name: %s, Gate's type: %s ", m->nodes[i]->gate_name, op);
                            for (l = 0; l < m->inputcount; l++)
                            {
                                if (strcmp(m->wires[k]->name, m->input_names[l]) == 0)
                                {
                                    results[next] = m->input_values[l];
                                    printf("\ninput wire: %s, value = %d", m->wires[k]->name, results[next]);
                                    next++;
                                }
                            }
                        }
                        else
                        {
                            results[next] = calculate(m, m->wires[k]);
                            // printf("Wire: %s, Value = %d", m->wires[k]->name, results[next]);
                            next++;
                        }
                        break;
                    }
                }
            }

            evaluate_operation(op, results, &gate_result);
            printf("\nGate's %s result =%d\n", m->nodes[i]->gate_name, gate_result);
        }
    }
    return gate_result;
}

bool solve(module *m, wire *output_wire) // circuit's output wire
{
    return calculate(m, output_wire);
}

void solving_circuit(module *m)
{
    int i = 0, j = 0;

    bool *outputs_values;
    outputs_values = (bool *)malloc(sizeof(bool) * m->outputcount);

    for (i = 0; i < m->outputcount; i++)
    {
        for (j = 0; j < m->wirecount; j++)
        {
            if (!strcmp(m->wires[j]->name, m->output_names[i]))
            {
                outputs_values[i] = solve(m, m->wires[j]);
                printf("\nOutput %s, value = %d", m->output_names[i], outputs_values[i]);
                break;
            }
        }
    }
}