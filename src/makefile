OBJ_DIR = obj
OBJ_DIR_DEBUG = dbg

PARSE_TREE_FILES := $(wildcard parse_tree/*.cpp)

CPP_FILES :=  $(wildcard *.cpp) \
							$(wildcard symbol_table/*.cpp) \
							$(wildcard csv/*.cpp) \
							$(PARSE_TREE_FILES)

PARSE_TREE_OBJS = $(addprefix obj/, $(notdir $(PARSE_TREE_FILES:.cpp=.o)))
DEBUG_PARSE_TREE_OBJS = $(addprefix dbg/, $(notdir $(PARSE_TREE_FILES:.cpp=.o)))

OBJ_FILES = $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
DEBUG_OBJ_FILES = $(addprefix dbg/, $(notdir $(CPP_FILES:.cpp=.o)))
#$(info $$var is [${DEBUG_OBJ_FILES}])

CXX_DEBUG_FLAGS = -DDEBUG

all: $(OBJ_FILES)

.PHONY: debug
debug: CXX_FLAGS += $(CXX_DEBUG_FLAGS)
debug: $(DEBUG_OBJ_FILES)

#release stuff
$(PARSE_TREE_OBJS): $(OBJ_DIR)/%.o: parse_tree/%.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

$(OBJ_DIR)/SymbolTable.o: symbol_table/SymbolTable.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

$(OBJ_DIR)/csv.o: csv/csv.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

$(OBJ_DIR)/%.o: %.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

#debug stuff
$(DEBUG_PARSE_TREE_OBJS): $(OBJ_DIR_DEBUG)/%.o: parse_tree/%.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

$(OBJ_DIR_DEBUG)/SymbolTable.o: symbol_table/SymbolTable.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

$(OBJ_DIR_DEBUG)/csv.o: csv/csv.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)

$(OBJ_DIR_DEBUG)/%.o: %.cpp
	g++ -c $< -o $@ -Wall $(CXX_FLAGS)


clean:
	rm $(OBJ_DIR)/*.o
	rm $(OBJ_DIR_DEBUG)/*.o
