PROGRAM_NAME = sod2png
COMPILER = gcc
CCFLAGS = -Wall -g
PATHFLAGS = -I/usr/include/libxml2 -I/usr/local/include/cairo
LLFLAGS = -lxml2 -lcairo -lsvg-cairo

sod2png: main.o arg.o editing.o svg2png.o types.o
	${COMPILER} ${CCFLAGS} ${PATHFLAGS} ${LLFLAGS} -o ${PROGRAM_NAME} main.o arg.o editing.o svg2png.o types.o

svg2png.o: svg2png.c svg2png.h
	gcc    -c ${PATHFLAGS} svg2png.c

editing.o: editing.c editing.h
	gcc    -c ${PATHFLAGS} editing.c

arg.o: arg.c arg.h
	gcc    -c ${PATHFLAGS} arg.c

types.o: types.c types.h
	gcc    -c ${PATHFLAGS} types.c

main.o: main.c arg.h editing.h 
	gcc    -c ${PATHFLAGS} main.c 	

clean: 
	rm -rf *.o sod2png
