PARSE_TREE_FILES := $(wildcard parse_tree/*.cpp)
PARSE_TREE_OBJS := $(addprefix obj/, $(notdir $(PARSE_TREE_FILES:.cpp=.o)))

CPP_FILES :=  $(wildcard *.cpp) \
							$(wildcard symbol_table/*.cpp) \
							$(wildcard csv/*.cpp) \
							$(PARSE_TREE_FILES)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
#$(info $$var is [${PARSE_TREE_FILES}])

all: $(OBJ_FILES)

$(PARSE_TREE_OBJS): obj/%.o: parse_tree/%.cpp
	g++ -c $< -o $@ -Wall

obj/SymbolTable.o: symbol_table/SymbolTable.cpp
	g++ -c $< -o $@ -Wall

obj/csv.o: csv/csv.cpp
	g++ -c $< -o $@ -Wall

obj/%.o: %.cpp
	g++ -c $< -o $@ -Wall

clean:
	rm obj/*.o
