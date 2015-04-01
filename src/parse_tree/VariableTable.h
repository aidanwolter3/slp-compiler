class VariableTable {
  private:
    class VariableTableEntry {
      public:
        char *lexem;
        int val;
        VariableTableEntry(char *l, int v) {lexem = l; val = v;};
    };

    int size;
    VariableTableEntry **entries;

  public:
    VariableTable();
    int get(char *lexem);
    void set(char *lexem, int val);
    void print();
};
