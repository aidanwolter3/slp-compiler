CPP_FILES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all: $(OBJ_FILES)
	g++ compiler.cpp $(OBJ_FILES) -o compiler 

$(OBJ_FILES):
	(cd src; make)
clean:
	(cd src; make clean)
	rm compiler
