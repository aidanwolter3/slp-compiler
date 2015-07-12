OBJ_DIR = src/obj/
OBJ_DIR_DEBUG = src/dbg/

CPP_FILES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR), $(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES_DEBUG := $(addprefix $(OBJ_DIR_DEBUG), $(notdir $(CPP_FILES:.cpp=.o)))

all:
	(cd src; make)
	g++ compiler.cpp $(OBJ_FILES) -o compiler

debug:
	(cd src; make debug)
	g++ compiler.cpp $(OBJ_FILES_DEBUG) -o compiler -DDEBUG

mac-exe:
	nasm -f macho64 output.asm
	ld -macosx_version_min 10.7.0 -o output output.o
ubu-exe:
	nasm -f elf output.asm -o output.o
	ld -m elf_i386 -s -o output output.o

atest:
	nasm -f macho testing.asm
	ld -macosx_version_min 10.7.0 -o testing testing.o
	./testing

clean:
	(cd src; make clean)
	rm *.o
	rm *.asm
	rm compiler
