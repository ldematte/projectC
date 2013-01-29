class TypeDef
{
public:

  TYPE_TYPE typeID;

  //types used by this one
  std::deque<TypePtr> typeList;

  //type modifiers
  bool isConst;
  bool isReference;
  bool isPointer;

  int arraySize;

  std::string name;

  SymPtr symbol;
};
