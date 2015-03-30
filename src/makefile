CPP_FILES :=  $(wildcard *.cpp) \
							$(wildcard symbol_table/*.cpp) \
							$(wildcard parse_csv/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
#$(info $$var is [${OBJ_FILES}])

all: $(OBJ_FILES)

obj/symbol_table.o: symbol_table/symbol_table.cpp
	g++ -c $< -o $@

obj/parse_csv.o: parse_csv/parse_csv.cpp
	g++ -c $< -o $@

obj/%.o: %.cpp
	g++ -c $< -o $@

#parse_tree: parse_tree/parse_tree.cpp
#	g++ -c -o obj/parse_tree.o

clean:
	rm obj/*.o
