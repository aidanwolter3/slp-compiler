OBJ_FILES := $(wildcard src/obj/*.o)

all:
	(cd src; make)
	g++ compiler.cpp $(OBJ_FILES) -o compiler 

clean:
	(cd src; make clean)
	rm compiler
