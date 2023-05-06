 
# Verilog parser makefile
.PHONY: clean

CFLAGS = -g -Wall -Wextra

my_parser: my_parser.o module_code.o my_help_functions.o circuit_code.o printing_functions.o keywords.o node_connections.o
	gcc ${CFLAGS} -o my_parser my_parser.o module_code.o my_help_functions.o circuit_code.o printing_functions.o keywords.o node_connections.o

keywords.o: keywords.h keywords.c
	gcc ${CFLAGS} -c keywords.c

my_parser.o: functions.h my_parser.c
	gcc ${CFLAGS} -c my_parser.c

module_code.o: keywords.o functions.h module_code.c
	gcc ${CFLAGS} -c module_code.c

my_help_functions.o: keywords.o functions.h my_help_functions.c
	gcc ${CFLAGS} -c my_help_functions.c

circuit_code.o: keywords.o functions.h circuit_code.c
	gcc ${CFLAGS} -c circuit_code.c

printing_functions.o: functions.h printing_functions.c
	gcc ${CFLAGS} -c printing_functions.c

node_connections.o: functions.h node_connections.c
	gcc ${CFLAGS} -c node_connections.c


clean:
	rm -f my_parser *.o
