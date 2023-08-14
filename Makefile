 
# Verilog parser makefile
.PHONY: clean

CC = gcc
CFLAGS = -g -Wall -Wextra
# -fno-stack-protector

parser: parser.o module_code.o help_functions.o circuit_code.o printing_functions.o keywords.o circuit_simulation.o
	${CC} ${CFLAGS} -o parser parser.o module_code.o help_functions.o circuit_code.o printing_functions.o keywords.o circuit_simulation.o

keywords.o: keywords.h keywords.c
	${CC} ${CFLAGS} -c keywords.c

parser.o: module.h functions.h parser.c
	${CC} ${CFLAGS} -c parser.c

module_code.o: keywords.o functions.h module_code.c
	${CC} ${CFLAGS} -c module_code.c

help_functions.o: keywords.o module_code.o module.h functions.h help_functions.c
	${CC} ${CFLAGS} -c help_functions.c

circuit_code.o: keywords.o module.h functions.h circuit_code.c
	${CC} ${CFLAGS} -c circuit_code.c

circuit_simulation.o: functions.h circuit_code.o circuit_simulation.c
	${CC} ${CFLAGS} -c circuit_simulation.c

printing_functions.o: module.h circuit_simulation.o printing_functions.c
	${CC} ${CFLAGS} -c printing_functions.c


clean:
	rm -f parser *.o
