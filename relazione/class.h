struct ClassHeader
{
  int size;
  tag_t tag;
  tag_t parentTag; 
  int objectSize;
  int functionCount;
  int virtualCount;
  int nativeCount;
  int[] vtable;
  FunctionEntry[] funcTable;
};

struct FunctionEntry
{
  int ordinal;
  int localCount;
  int paramCount;
  union
  {
    int startAddress;
    tag_t nativeID;
  };
  bool isNative;
};
