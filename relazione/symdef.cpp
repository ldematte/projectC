
class SymDef
{
public:
  //the name of the symbol
  std::string name;

  //the type of the symbol
  TypePtr type;

  //the owner of this symbol (scope)
  SymPtr parent;

  //symbol kind (class, variable, function...)
  SYMBOL_KIND symKind;

  //the list of "sons" (symbols declared in this scope)
  std::list<SymPtr> declList;

  //CLASS MEMBERS
  //NAMESPACE MEMBERS
  //SCOPEBLOCK MEMBERS
  //FUNCTION MEMBERS
  //VARIABLE  MEMBERS
  //TYPEDEF MEMBERS
};


