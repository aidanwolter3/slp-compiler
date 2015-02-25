all: syn.cpp symbol_table.o
	gcc syn.cpp symbol_table.o -o syn
	./syn test

symbol_table.o: symbol_table.cpp
	gcc -c symbol_table.cpp -o symbol_table.o
