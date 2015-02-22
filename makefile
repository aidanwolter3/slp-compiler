all: syn.cpp
	gcc syn.cpp -o syn
	cat test | ./syn
