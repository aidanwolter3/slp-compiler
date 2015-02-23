all: syn.cpp
	gcc syn.cpp -o syn
	./syn test
