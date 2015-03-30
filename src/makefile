PARSE_TREE_FILES := $(wildcard parse_tree/*.cpp)
PARSE_TREE_OBJS := $(addprefix obj/, $(notdir $(PARSE_TREE_FILES:.cpp=.o)))

CPP_FILES :=  $(wildcard *.cpp) \
							$(wildcard symbol_table/*.cpp) \
							$(wildcard parse_csv/*.cpp) \
							$(PARSE_TREE_FILES)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
#$(info $$var is [${PARSE_TREE_FILES}])

all: $(OBJ_FILES)

$(PARSE_TREE_OBJS): obj/%.o: parse_tree/%.cpp
	g++ -c $< -o $@

obj/symbol_table.o: symbol_table/symbol_table.cpp
	g++ -c $< -o $@

obj/parse_csv.o: parse_csv/parse_csv.cpp
	g++ -c $< -o $@

obj/%.o: %.cpp
	g++ -c $< -o $@

clean:
	rm obj/*.o
