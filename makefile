CPP_FILES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all:
	(cd src; make)
	g++ compiler.cpp $(OBJ_FILES) -o compiler 

atest:
	nasm -f macho assembly_test.asm && ld -macosx_version_min 10.7.0 -o assembly_test assembly_test.o && ./assembly_test

clean:
	(cd src; make clean)
	rm compiler
