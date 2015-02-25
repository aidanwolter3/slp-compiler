all: syn.cpp symbol_table.o lexical_analyzer.o parse_csv.o
	g++ syn.cpp symbol_table.o lexical_analyzer.o parse_csv.o -o syn
	./syn test

symbol_table.o: symbol_table.cpp
	g++ -c symbol_table.cpp -o symbol_table.o

lexical_analyzer.o: lexical_analyzer.cpp
	g++ -c lexical_analyzer.cpp -o lexical_analyzer.o

parse_csv.o: parse_csv.cpp
	g++ -c parse_csv.cpp -o parse_csv.o

clean:
	rm *.o
	rm syn
