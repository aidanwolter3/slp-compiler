CPP_FILES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all:
	(cd src; make)
	g++ compiler.cpp $(OBJ_FILES) -o compiler 

clean:
	(cd src; make clean)
	rm compiler
