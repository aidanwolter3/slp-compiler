CPP_FILES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all:
	(cd src; make)
	g++ compiler.cpp $(OBJ_FILES) -o compiler

mac-exe:
	nasm -f macho output.asm
	ld -macosx_version_min 10.7.0 -o output output.o
ubuntu-exe:
	nasm -f elf output.asm -o output.o
	ld -m elf_i386 -s -o output output.o

atest:
	nasm -f macho testing.asm
	ld -macosx_version_min 10.7.0 -o testing testing.o
	./testing

clean:
	(cd src; make clean)
	rm *.o
	rm compiler
