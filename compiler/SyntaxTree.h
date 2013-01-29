
#ifndef SYNTAX_TREE_H_INCLUDED_
#define SYNTAX_TREE_H_INCLUDED_

#include <string>
#include <iostream>
#include <typeinfo>

#include "type.h"
#include "symbol.h"
#include "ICodeEmitter.h"
#include "ScopeManager.h"
#include "constants.h"
#include "ErrorManager.h"
#include "OutputManager.h"

/*
oggetti del namespace globale come abs stack index,
poi ho codice suddiviso a livello di funzione

ha senso avere (OGGETTO, NUMERO) quando NUMERO
potrebbe anch'esso riferirsi a un ogetto?

Ho bisogno di una LEA o qualcosa di simile

class A 
*/
/*

A a = new B;

NEW REG_A, type(B)

  NEW REG_B, type(B::a)
  MOV REG_A.a REG_B

MOV a, REG_A

a: variabile, objAddr (Operand con type_id = OP_TYPE_HEAP_ADDR)

*/

#define IE           ICodeEmitter::GetSingleton()
#define TR           TypeRoot::GetSingleton()
#define scope_stack  ScopeManager::GetSingleton()

class ISyntaxNode;
class DeclNode;
class SyntaxTree;
typedef ISyntaxNode* NodePtr;

//this is a syntax tree

//type of nodes we can found in a syntax tree
//see minicpp2.y (minicpp.cpp.h)
typedef int NODE_TYPE;

class ISyntaxNode
{
public:

  virtual bool isLeaf() { return (sons.size() == 0 ? true : false); }

  //sons in this binary tree  
  std::vector<NodePtr> sons;
  
  void addSon(NodePtr son)
  {
    sons.push_back(son);
  }

  //type from the parser: NODE_TYPE
  NODE_TYPE type;

  //every node must have a type expression
  //definition of type
  //(pointer to a node in the type tree)
  TypePtr typeRec;
  TypePtr getType() { return typeRec; }

  //Some nodes (e.g. identifier definition) have a symbol associated
  SymPtr symbol;

  virtual TypePtr checkType() = 0;
  virtual void generateCode() = 0;
  virtual Operand evaluteExpr() = 0;

  virtual void propagateTypeInfo(TypePtr typeInfo, bool isStatic) { }
  virtual SymPtr getSymbol() { return symbol; }

  //must be spacialized in nodes that return a scope, as
  //in member accessing node
  virtual SymPtr resolveScope() { return symbol; }

  //nodes normally does not evalute directly to an int.
  //this must be spacialized in const expressions.
  virtual bool evaluteInt(int& evalutedValue) { return false; }

  //nodes normally does not evalute directly to a boolean
  //this must be spacialized in const expressions.
  virtual bool evaluteBool(bool& evalutedValue) { return false; }

  virtual void dump(int level = 0)
  {
    std::string header;
    for (int i = 0; i < level; ++i)
      header += "--";
    header += "> ";

    std::cout << header << "Leaf : " << typeid(*this).name() << std::endl;
  }
  
  int getLineNumber();
  void setLineNumber(int lineNum);
  
  virtual ~ISyntaxNode() { }

};

class SyntaxNode : public ISyntaxNode
{
public:

  virtual ~SyntaxNode()
  {
    //scende ricorsivamente nelle foglie
    for (std::vector<NodePtr>::iterator i = sons.begin(); i != sons.end(); ++i)
      delete (*i);
  }

  virtual void dump(int level = 0)
  {
    std::string header;
    for (int i = 0; i < level; ++i)
      header += "--";
    header += "> ";

    std::cout << header << "Node : " << typeid(*this).name() << std::endl;
    std::cout << header << "Type : " << typeRec->getName() << std::endl;

    for (std::vector<NodePtr>::iterator it = sons.begin(); it != sons.end(); ++it)
      (*it)->dump(level + 1);
  }

  virtual void generateCode() 
  {
    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->generateCode();
    }
  }

  virtual Operand evaluteExpr()
  {
    assert(0);
    return Operand();
  }

};

class SyntaxExprNode : public SyntaxNode
{
public:


  virtual void generateCode() 
  {
    evaluteExpr();
  }

};


class SyntaxTree : public Singleton<SyntaxTree>, public SyntaxNode
{
/*
private:
  NodePtr createNode(int sons, int type);
*/
public:

  //NodePtr createClass();

  virtual TypePtr checkType() 
  {
    if (sons.size())
      typeRec = sons[0]->checkType();

    for (size_t i = 1; i < sons.size(); ++i)
    {
      TypePtr sonType = sons[i]->checkType();
      
      if (typeRec->isCompatible(sonType) || sonType->isCompatible(typeRec))
        typeRec = sonType;
      else
        typeRec = TR.getErr();
    }
    return typeRec;
  }
  virtual void generateCode() 
  {
    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->generateCode();
    }
  }

  //WARNING! the content of regC is destroyed by the
  //following function
  void generateNew(TypePtr class_type)
  {
    if (class_type->typeID == TYPE_CLASS)
    {
      SymPtr class_symbol = class_type->getSymbol();

      Operand regC, regD;
      regC.regCode = REG_C;
      regD.regCode = REG_D;
      regC.type_id = regD.type_id = OP_TYPE_REG;

      xasm(OP_NEW, regC, class_type);

      std::list<SymPtr>::iterator it;
      for (it = class_symbol->declList.begin(); it != class_symbol->declList.end(); ++it)
      {
        if ( ((*it)->symKind == SYM_VAR) && 
          ((*it)->type->typeID == TYPE_CLASS || (*it)->type->typeID == TYPE_STRING) )
        {    
          xasm(OP_PUSH, regC);
          xasm(OP_PUSH, regD);
          xasm(OP_LEA, regD, regC);
          xasm(OP_PADD, regD, regD, (*it)->getClassOffset());

          generateNew((*it)->type);

          xasm(OP_STORE, regD, regC);
          xasm(OP_POP, regD);
          xasm(OP_POP, regC);
        }
        else if ((*it)->symKind == SYM_VAR && (*it)->type->typeID == TYPE_ARRAY)
        {
          xasm(OP_PUSH, regC);
          xasm(OP_PUSH, regD);
          xasm(OP_LEA, regD, regC);
          xasm(OP_PADD, regD, regD, (*it)->getClassOffset());

          generateNewArray((*it)->type->getArrayElemType(), (*it)->type->arraySize);

          xasm(OP_STORE, regD, regC);
          xasm(OP_POP, regD);
          xasm(OP_POP, regC);
        }
      }
    }
    else if (class_type->typeID == TYPE_STRING)
    {
      Operand regC, opType;
      regC.regCode = REG_C;
      regC.type_id = OP_TYPE_REG;
      opType.type_id = OP_TYPE_TYPE_TAG;
      opType.typeTag = TYPE_STRING;

      xasm(OP_NEW, regC, opType);

    }
    else
    {
      //raise error
      std::string s = "operator new: wrong operand type";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      assert(0);
    }
  }

  //WARNING! the content of regC is destroyed by the
  //following function
  void generateNewArray(TypePtr elem_type, Operand elem_count)
  {
    Operand regC;
    regC.regCode = REG_C;
    regC.type_id = OP_TYPE_REG;

    //construct the array itself
    xasm(OP_NEWA, regC, elem_type, elem_count);

    if (!(elem_type->isPrimitive()))
    {
      //generate a loop to construct all the inside objects
      /*
      while (i < arraySize)
      {
      OP_NEW

      OP_STORE base + i
      }
      */
      ICodeNode loopBegin = IE.generateJumpTarget();
      ICodeNode loopEnd = IE.generateJumpTarget();

      //PUSH regA
      //PUSH regB
      //PUSH regC
      //PUSH regD;
      Operand regA, regB, regD;
      regA.regCode = REG_A;
      regB.regCode = REG_B;
      regD.regCode = REG_D;
      regA.type_id = regB.type_id = regD.type_id = OP_TYPE_REG;

      xasm(OP_PUSH, regA);
      xasm(OP_PUSH, regB);
      xasm(OP_PUSH, regD);

      xasm(OP_SET, regB, elem_count);
      xasm(OP_SET, regD, 0);

      //TODO: check if header is calculated or not: LEA?
      xasm(OP_PADD, regA, regC, ARRAYOBJ_HEADER_SIZE);

      IE.addJumpTarget(loopBegin);

      xasm(OP_JZ, regB, loopEnd.jumpTarget);
      xasm(OP_DEC, regB);

      xasm(OP_PADD, regA, regA, regD);
      xasm(OP_ADD, regD, regD, WORD_SIZE);

      xasm(OP_PUSH, regC);
      if (elem_type->typeID == TYPE_ARRAY)
      {
        generateNewArray(elem_type->getArrayElemType(), elem_type->arraySize);
      }
      else if ((elem_type->typeID == TYPE_CLASS) || 
        (elem_type->typeID == TYPE_STRING) )
      {
        generateNew(elem_type);
      }
      else assert(0);

      xasm(OP_STORE, regA, regC);
      xasm(OP_POP, regC);

      xasm(OP_JMP, loopBegin.jumpTarget);

      IE.addJumpTarget(loopEnd);

      xasm(OP_POP, regD);
      xasm(OP_POP, regB);
      xasm(OP_POP, regA);
    }
  }
};


class CastNode : public SyntaxExprNode
{
private:
  TypePtr oldType;

public:
  CastNode(TypePtr new_type, NodePtr expr_to_cast) : newType(new_type)
  {
    addSon(expr_to_cast);
  }

  virtual TypePtr checkType() 
  {
    typeRec = oldType = sons[0]->checkType();
    if (typeRec->isCastable(newType))
    {
      typeRec = newType;
    }
    else
    {
      //raise error
      std::string s = "Cannot cast form \"" + oldType->getName() + "\" to \"" + newType->getName() + "\"";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      typeRec = TR.getErr();
    }
    return typeRec;
  }

  virtual Operand evaluteExpr()
  {
    Operand op = sons[0]->evaluteExpr();

    Operand ret;
    ret.type_id = OP_TYPE_REG;
    if (op.type_id == OP_TYPE_REG && op.regCode == REG_C)
      ret.regCode = REG_D;
    else
      ret.regCode = REG_C;

    if (typeRec->isPrimitive())
    {
      if (oldType->typeID == TYPE_STRING)
      {
        if (newType->typeID == TYPE_INT)
          xasm(OP_STR2I, ret, op);
        else if (newType->typeID == TYPE_FLOAT)
          xasm(OP_STR2F, ret, op);
        else
          assert(0);
        return ret;
      }
      else if (oldType->typeID == TYPE_INT || oldType->typeID == TYPE_BOOL)
      {
        if (newType->typeID == TYPE_STRING)
          xasm(OP_I2STR, ret, op);
        else if (newType->typeID == TYPE_FLOAT)
          xasm(OP_I2F, ret, op);
        else if ((newType->typeID == TYPE_INT || newType->typeID == TYPE_BOOL))
          return op;
        else
          assert(0);          
        return ret;
      }
      else if (oldType->typeID == TYPE_FLOAT)
      {
        if (newType->typeID == TYPE_STRING)
          xasm(OP_F2STR, ret, op);
        else if (newType->typeID == TYPE_INT)
          xasm(OP_F2I, ret, op);
        else
          assert(0);    
        return ret;
      }
      else
        assert(0);
    }
    else if ((oldType->getSymbol()->isMyAncestor(newType->getSymbol())) &&
              !(newType->isReference))
    {
      xasm(OP_UCAST, ret, op, typeRec);
    }
    else if (newType->getSymbol()->isMyAncestor(oldType->getSymbol()))
    {
      xasm(OP_DCAST, ret, op, typeRec);
    }
    else
    {
      //raise error
      //no cast possible
      std::string s = "Cannot cast form \" " + oldType->getName() + "\" to \"" + newType->getName() + "\"";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
    }

    return ret;
  }
  
private:
  TypePtr newType;
};


//
//Blocks
///////////////////////////////////////////
class DeclBlock : public SyntaxNode
{
public:
  DeclBlock(NodePtr block) { addSon(block); }

  virtual TypePtr checkType() 
  { 
    typeRec = sons[0]->checkType(); 
    return typeRec; 
  }
  
};


//the following three class are the roots
//every piece of i-code is bound to a class-function
//and they are also needed to hold the scope
class ClassNode : public SyntaxNode
{
private:
  SymPtr classSymbol;
public:
  ClassNode(NodePtr body, SymPtr new_class) 
  { 
    addSon(body); 
    classSymbol = new_class; 
  }

  virtual TypePtr checkType() 
  { 
    typeRec = sons[0]->checkType(); 
    return typeRec; 
  }
  
  virtual void generateCode()   
  {
    scope_stack.pushScope(classSymbol);
    //TODO2: call constructor
    sons[0]->generateCode();
    scope_stack.popScope();
  }
};

class ClassBodyNode : public SyntaxNode
{
public:
  ClassBodyNode(NodePtr def, NodePtr other_defs)
  { 
    addSon(def); 
    addSon(other_defs);
  }

  virtual TypePtr checkType() 
  {
    if (!(sons[0]->checkType()->isError()) && !(sons[1]->checkType()->isError()))
      typeRec = TypeRoot::GetSingleton().getStdType(TYPE_OK);
    else
      typeRec = TypeRoot::GetSingleton().getErr();

    return typeRec;
  }

};

class FunctionDefNode : public SyntaxNode
{
private:
  SymPtr funcSymbol;

public:
  FunctionDefNode(NodePtr body, SymPtr func_symbol)
  { 
    addSon(body); 
    funcSymbol = func_symbol;
    typeSignature = func_symbol->type;
    typeRec = typeSignature->getRetType();
  }

  virtual TypePtr checkType() 
  {
    typeRec = sons[0]->checkType();
    //e' possibile una conversione dal tipo del corpo
    //al tipo della segnatura (dichiarazione)
    if (!typeRec->isCompatible(typeSignature->getRetType()))
      typeRec = TypeDef::createError();
    return typeRec;
  }

  virtual void generateCode() 
  {
    scope_stack.pushScope(funcSymbol);
    sons[0]->generateCode();
    scope_stack.popScope();
  }

private:
  TypePtr typeSignature;
};

class ParamNode : public SyntaxNode
{
public:
  ParamNode(NodePtr nextParam) { addSon(nextParam); }

  virtual TypePtr checkType() {}
};

//
// Primitive types
//////////////////////////////////////////////////////////////////////////

class IntNode : public SyntaxExprNode
{
public:
  IntNode(int value) : Int(value) 
  { 
  }
  
  virtual bool isLeaf() { return true; }
  virtual TypePtr checkType() 
  { 
    typeRec = TypeRoot::GetSingleton().getStdType(TYPE_INT);
    return typeRec;
  }

  virtual Operand evaluteExpr()
  {
    return Operand(Int);
  }

  int Int;
};

class FloatNode : public SyntaxExprNode
{
public:
  FloatNode(float value) : Float(value) { }
  
  virtual bool isLeaf() { return true; }
  virtual TypePtr checkType() 
  { 
    typeRec = TypeRoot::GetSingleton().getStdType(TYPE_FLOAT); 
    return typeRec;
  }
  
  virtual Operand evaluteExpr()
  {
    return Operand(Float);
  }
  
  float Float;
};

class BoolNode : public SyntaxExprNode
{
public:
  BoolNode(bool value) : Bool(value) { }
  
  virtual bool isLeaf() { return true; }
  virtual TypePtr checkType() 
  { 
    typeRec = TypeRoot::GetSingleton().getStdType(TYPE_BOOL); 
    return typeRec;
  }
  virtual Operand evaluteExpr()
  {
    return Operand(Bool);
  }
  
  bool Bool;
};

class StringNode : public SyntaxExprNode
{
public:
  StringNode(char* value) : String(value) { }
  
  virtual bool isLeaf() { return true; }
  virtual TypePtr checkType() 
  {
    typeRec = TypeRoot::GetSingleton().getStdType(TYPE_STRING); 
    return typeRec;
  }

  virtual Operand evaluteExpr()
  {
    return Operand(String);
  }
  
  std::string String;
};

class DeclListNode : public SyntaxNode
{
public:

  DeclListNode(TypePtr type, NodePtr idList, bool isStatic) 
    : declStatic(isStatic) 
  { 
    typeRec = type;
    addSon(idList);
  }
  
  virtual TypePtr checkType() 
  { 
    //ritorna il tipo dichiarato
    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->checkType();
    }
    return typeRec;
  }

  //propagate type info to sons
  void propagateTypeInfo(TypePtr typeInfo, bool isStatic)
  {
    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->propagateTypeInfo(typeRec, declStatic); //typeInfo
    }
  }

  virtual void generateCode()
  {
    evaluteExpr();
  }

  virtual Operand evaluteExpr()
  {
    return sons[0]->evaluteExpr();
  }

private:
  bool    declStatic;
};

class DeclNode : public SyntaxNode
{
public: 
  DeclNode(NodePtr decl1, NodePtr decl2) { addSon(decl1); addSon(decl2); }
  DeclNode(NodePtr decl1) { addSon(decl1); }

  virtual TypePtr checkType() 
  { 
    //ritorna il tipo dichiarato
    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->checkType();
    }
    return typeRec;
  }

  //propagate type info to sons
  void propagateTypeInfo(TypePtr typeInfo, bool isStatic)
  {
    typeRec = typeInfo;
    this->isStatic = isStatic;

    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->propagateTypeInfo(typeInfo, isStatic);
    }
  }

  virtual void generateCode()
  {
    evaluteExpr();
  }

  virtual Operand evaluteExpr()
  {
    if (sons.size() == 2)
    {
      sons[0]->evaluteExpr();
      return sons[1]->evaluteExpr();
    }
    else
      return sons[0]->evaluteExpr();
  }

private:
  bool isStatic;
};

class InitDeclNode : public SyntaxNode
{
private:
  SymPtr varSymbol;

public: 
  InitDeclNode(NodePtr var_decl, NodePtr init) 
  { 
    addSon(var_decl); addSon(init); 
    varSymbol = var_decl->getSymbol();
  }
  
  virtual TypePtr checkType() 
  { 
    TypePtr type1 = sons[0]->checkType();
    TypePtr type2 = sons[1]->checkType();

    if ( (!type1->isError()) && !(type2->isError()) )
      if (type2->isCompatible(type1)) //NB: one way in this case (only type2 -> type1)
      {
        //controlla anche che left side sia assegnabile
        //non const e != string literal
        if (!type2->isEqual(type1))
        {
          //insert conversion code
          NodePtr conversionNode = new CastNode(type1, sons[1]);
          conversionNode->checkType();
          sons[1] = conversionNode;
        }

        typeRec = type1;
        return typeRec;
      }

    //raise error
    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    typeRec = TR.getErr();
    return typeRec;    

    /*
    assert(typeRec == sons[0]->checkType());
    if (!typeRec->isEqual(sons[1]->checkType()))
      typeRec = TypeDef::createError();
    return typeRec;
    */
  }

  virtual void generateCode() 
  { 
    //we need to assign the expression in init to 
    //the variable declared in var_decl
    Operand op = sons[1]->evaluteExpr();

    xasm(OP_MOV, varSymbol, op);
  };

  virtual Operand evaluteExpr() 
  { 
    //we need to assign the expression in init to 
    //the variable declared in var_decl
    Operand op = sons[1]->evaluteExpr();

    xasm(OP_MOV, varSymbol, op);

    return op;
  };

  //propagate type info to sons
  void propagateTypeInfo(TypePtr typeInfo, bool isStatic)
  {
    typeRec = typeInfo;
    this->isStatic = isStatic;

    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      (*i)->propagateTypeInfo(typeInfo, isStatic);
    }
  }

private:
  bool isStatic;

};

class AssignInitNode : public SyntaxNode
{
public: 
  AssignInitNode(NodePtr init_expr) 
  { 
    addSon(init_expr);
  }
  
  virtual TypePtr checkType() 
  { 
    typeRec = sons[0]->checkType(); 
    return typeRec; 
  }

  //propagate type info to sons
  void propagateTypeInfo(TypePtr typeInfo, bool isStatic) {  }  

  virtual Operand evaluteExpr()
  {
    return sons[0]->evaluteExpr();
  }
};

class InitCtorNode : public SyntaxNode
{
public: 
  InitCtorNode(NodePtr init_expr_list) 
  { 
    addSon(init_expr_list);
  }
  
  virtual TypePtr checkType() { typeRec = sons[0]->checkType(); return typeRec; }

  //propagate type info to sons
  void propagateTypeInfo(TypePtr typeInfo, bool isStatic) {  }  
};

class VarDeclNode : public SyntaxExprNode
{
public:

  VarDeclNode(SymPtr new_var)
  { 
    symbol = new_var;
  }

  VarDeclNode(SymPtr new_var, NodePtr expr_init)
  { 
    symbol = new_var;
    addSon(expr_init);
  } 
  
  virtual bool isLeaf() { return (sons.size() == 0 ? true : false); }
  virtual TypePtr checkType() 
  { 
    typeRec = symbol->type;
    return typeRec; 
  }

  void propagateTypeInfo(TypePtr typeInfo, bool isStatic)
  {
    if (symbol->type && (symbol->type->typeID == TYPE_ARRAY || symbol->type->typeID == TYPE_PROD))
      symbol->type->addType(typeInfo);
    else
      symbol->type = typeInfo;

    typeRec = symbol->type;
    symbol->isStatic = isStatic;
  }

  virtual Operand evaluteExpr()
  {
    switch (typeRec->typeID)
    {
    case TYPE_CLASS:
    case TYPE_STRING: //e' uguale al type class? si, si distingue nella VM
      {
        Operand regC;
        regC.regCode = REG_C;
        regC.type_id = OP_TYPE_REG;

        //costruisci tutti i tipi contenuti
        SyntaxTree::GetSingleton().generateNew(symbol->type);

        xasm(OP_STORE, symbol, regC);
      }
      break;
    case TYPE_ARRAY:
      {
        Operand regC;
        regC.regCode = REG_C;
        regC.type_id = OP_TYPE_REG;

        //costruisci tutti i tipi contenuti
        SyntaxTree::GetSingleton().generateNewArray(typeRec->getArrayElemType(), typeRec->arraySize);

        xasm(OP_STORE, symbol, regC);
      }
      break;

    default:
      break;
    }

    return Operand(symbol);

  }
};


class VarDeclArrayNode : public SyntaxExprNode
{
public:

  VarDeclArrayNode(NodePtr variableOrArray, SymPtr arraySym) 
  { 
    symbol = arraySym;
    addSon(variableOrArray); 
  }
  
  virtual bool isLeaf() { return (sons.size() == 0 ? true : false); }
  virtual TypePtr checkType() 
  { 
    typeRec = symbol->type;
    return typeRec; 
  }

  void propagateTypeInfo(TypePtr typeInfo, bool isStatic)
  {
    //TypePtr arrayType = TypeDef::createArray(typeInfo, symbol->type->arraySize);
    //arrayType->arraySize = typeRec->arraySize;
    //typeRec = arrayType;
    //typeRec->addSon(typeInfo)?

    std::vector<NodePtr>::iterator i;
    for (i = sons.begin(); i != sons.end(); ++i)
    {
      //(*i)->propagateTypeInfo(arrayType, isStatic);
      (*i)->propagateTypeInfo(typeInfo, isStatic);
    }
  }

  virtual Operand evaluteExpr()
  {
    Operand op = sons[0]->evaluteExpr();  
    return op;
  }
};

class FunctionDeclNode : public SyntaxNode
{
public:
  FunctionDeclNode(SymPtr new_var)
  { 
    symbol = new_var;
  }
 
  virtual bool isLeaf() { return (sons.size() == 0 ? true : false); }
  virtual TypePtr checkType() 
  { 
    typeRec = symbol->type->getRetType();
    return typeRec;
  }

  void propagateTypeInfo(TypePtr typeInfo, bool isStatic)
  {
    if (symbol->type && (symbol->type->typeID == TYPE_ARRAY || symbol->type->typeID == TYPE_PROD))
      symbol->type->addType(typeInfo);
    else
      symbol->type = typeInfo;

    symbol->isStatic = isStatic;
  }
};

class IdNode : public SyntaxExprNode
{
public:
  IdNode(SymPtr symbol)
  { 
    this->symbol = symbol;
  }

  Operand evaluteExpr()
  {
    return Operand(symbol);
  }
};



//
// Statements
/////////////////////////////////////////////////////

class StatementListNode : public SyntaxNode
{
public:

  StatementListNode(NodePtr statement) { addSon(statement); }
  StatementListNode(NodePtr statement, NodePtr other_stmts) { addSon(statement); addSon(other_stmts); }

  virtual TypePtr checkType() 
  { 
    if (sons.size() > 1)
    {
      typeRec = sons[0]->checkType();
      if (!typeRec->isError())
        typeRec = sons[1]->checkType();
      return typeRec;
    }
    else
    {
      typeRec = sons[0]->checkType();      
      return typeRec;
    }
  }
};

/*
class MemberUseNode : public SyntaxExprNode
{
public:

  MemberUseNode(SymPtr member) 
  { 
    symbol = member;
    typeRec = symbol->type; 
  }

  virtual TypePtr checkType() { return symbol->type; }
  
  virtual Operand evaluteExpr()
  {
    return Operand(symbol);
  }
};
*/

class ITENode : public SyntaxNode
{
public:
  ITENode(NodePtr expr, NodePtr Then)
  {
    addSon(expr);
    addSon(Then);
  }

  ITENode(NodePtr expr, NodePtr Then, NodePtr Else)
  {
    addSon(expr);
    addSon(Then);
    addSon(Else);
  }

  virtual TypePtr checkType() 
  {
    //check expr against boolean, 
    //and statement correctness

    if (sons[0]->checkType()->isCompatible(TR.getStdType(TYPE_BOOL)))
    {
      if (!(sons[1]->checkType()->isError()) && !(sons[2]->checkType()->isError()) )
      {
        typeRec = TR.getStdType(TYPE_OK);
        return typeRec;
      }
    }
        
    //raise error
    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    typeRec =  TR.getErr();
    return typeRec;
  }

  virtual void generateCode()
  {
    ICodeNode target1 = IE.generateJumpTarget();
    ICodeNode target2 = IE.generateJumpTarget();

    if (sons.size() == 3)
    {
      //if
      Operand op1 = sons[0]->evaluteExpr();
      xasm(OP_JZ, op1, target1.jumpTarget);
      //then
      sons[1]->generateCode();
      xasm(OP_JMP, target2.jumpTarget);
      //else
      IE.addJumpTarget(target1);
      sons[2]->generateCode();
      IE.addJumpTarget(target2);
    }
    else
    {
      ICodeNode target1 = IE.generateJumpTarget();
      ICodeNode target2 = IE.generateJumpTarget();

      //if
      Operand op1 = sons[0]->evaluteExpr();
      xasm(OP_JZ, op1, target1.jumpTarget);
      //then
      sons[1]->generateCode();

      IE.addJumpTarget(target1);
    }
  }
};

class CaseNode : public SyntaxNode
{
public:

  CaseNode(NodePtr caseNumber, NodePtr statement) 
    : default_(false) { addSon(statement); addSon(caseNumber); }

  CaseNode(NodePtr statement)
    : default_(true) { addSon(statement); }

  virtual TypePtr checkType() 
  { 
    if (!default_)
    {
      if (!(sons[0]->checkType())->isEqual(TypeRoot::GetSingleton().getStdType(TYPE_INT)))
      {
        typeRec = TypeDef::createError();
        return typeRec; 
      }
      typeRec = sons[1]->checkType();
      return typeRec;
    }
    else
    {
      typeRec = sons[0]->checkType();
      return typeRec;
    }
  }

  virtual void generateCode() 
  { 
    if (IE.loopStack.empty())
    {
      //raise error
      std::string s = "case statement without switch";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      return;
    }
    
    ICodeNode target = IE.generateJumpTarget();
    ICodeEmitter::Bookmark& sb = IE.getCurrentLoop();

    if (sb.hasDefault)
    {
      //raise error: WARNING unreachable code
      //switch already has a default label
      OutputManager::GetSingleton().out(OutputManager::WARNING, "unreachable code: switch already has a default label");
    }
    else
    {
      //inserisce il codice per il jump all'inizio dello switch
      if (default_)
      {
        IE.addInstr(OP_JMP, next(sb.firstInstr));
        IE.addOperand(target.jumpTarget);
        sb.hasDefault = true;
      }
      else
      {
        Operand op = sons[1]->evaluteExpr();
        IE.addInstr(OP_JE, next(sb.firstInstr));
        IE.addRegOperand(REG_A);
        IE.addOperand(op);
        IE.addOperand(target.jumpTarget);
      }
      ++sb.firstInstr;

      IE.addJumpTarget(target);
      sons[0]->generateCode();
    }
  }

private:
  int number_;
  bool default_;
};

class SwitchNode : public SyntaxNode
{
public:
  SwitchNode(NodePtr expr, NodePtr stmt)
  {
    addSon(expr);
    addSon(stmt);
  }

  virtual TypePtr checkType() 
  {
    if (sons[0]->checkType()->isCompatible(TR.getStdType(TYPE_INT)))
    {
      //insert appropriate conversion, if necessary
      if (!sons[0]->typeRec->typeID == TYPE_INT)
      {
        NodePtr conversionNode = new CastNode(TR.getStdType(TYPE_INT), sons[0]);
        conversionNode->checkType();
        sons[0] = conversionNode;
      }

      if (!sons[1]->checkType()->isError())
      {
        typeRec = TR.getStdType(TYPE_OK);
        return typeRec;
      }
    }

    //raise error
    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    typeRec = TR.getErr();
    return typeRec;
  }
  virtual void generateCode() 
  {
    //generate code for the expression
    //the result is in EAX
    Operand op = sons[0]->evaluteExpr();
    IE.addInstr(OP_MOV);
    IE.addRegOperand(REG_A);
    IE.addOperand(op);
    
    //create a bookmark
    //the case(s) and break(s) will use this bookmark
    //to place instruction in correct places.
    ICodeNode switchEnd = IE.generateJumpTarget();
    IE.pushLoop(IE.getCurrentInstr(), switchEnd);
  
    //generate code of statement  
    sons[1]->generateCode();

    IE.addJumpTarget(switchEnd);
    IE.popLoop();
  }
};

class ContinueNode : public SyntaxNode
{
public:
  ContinueNode() { }

  virtual TypePtr checkType() 
  {
    return TR.getStdType(TYPE_OK);
  }

  virtual void generateCode() 
  {
    if (IE.loopStack.empty())
    {
      //raise error
      std::string s = "continue statement outside a loop";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
      return;
    }

    ICodeEmitter::Bookmark sb = IE.getCurrentLoop();
    
    //see if the first instr is a jump target
    if (sb.firstInstr->tag != JUMP_TARGET)
    {
      //raise error
      //is due to the while to make this a jump target
      std::string s = "Internal compiler error: WHILE didn't made a jump target";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
    }

    IE.addInstr(OP_JMP);
    IE.addOperand(sb.firstInstr->jumpTarget);
  }
};

class BreakNode : public SyntaxNode
{
public:
  BreakNode() { }

  virtual TypePtr checkType() 
  {
    return TR.getStdType(TYPE_OK);
  }

  virtual void generateCode() 
  {
    if (IE.loopStack.empty())
    {
      //raise error
      std::string s = "break statement outside a loop (or switch)";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
      return;
    }

    ICodeEmitter::Bookmark& sb = IE.getCurrentLoop();
    IE.addInstr(OP_JMP);
    assert(sb.endTarget.tag == JUMP_TARGET);
    IE.addOperand(sb.endTarget.jumpTarget);
  }
};

class WhileNode : public SyntaxNode
{
public:
  WhileNode(NodePtr expr, NodePtr stmt)
  {
    addSon(expr);
    addSon(stmt);
  }

  virtual TypePtr checkType() 
  {
    if (sons[0]->checkType()->isCompatible(TR.getStdType(TYPE_BOOL)))
    {
      if (!sons[1]->checkType()->isError())
      {
        typeRec = TR.getStdType(TYPE_OK);
        return typeRec;
      }
    }

    //raise error
    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());
    
    typeRec = TR.getErr();
    return typeRec;
  }

  virtual void generateCode() 
  {
    ICodeNode loopBegin = IE.generateJumpTarget();
    ICodeNode loopEnd = IE.generateJumpTarget();

    //the label to the start of the loop
    IE.addJumpTarget(loopBegin);
    IE.pushLoop(IE.getCurrentInstr(), loopEnd);

    Operand op = sons[0]->evaluteExpr();

    IE.addInstr(OP_JZ);
    IE.addOperand(op);
    IE.addOperand(loopEnd.jumpTarget);

    sons[1]->generateCode();
    IE.addInstr(OP_JMP);
    IE.addOperand(loopBegin.jumpTarget);

    IE.addJumpTarget(loopEnd);
    IE.popLoop();
  }
};

class DoWhileNode : public SyntaxNode
{
public:
  DoWhileNode(NodePtr expr, NodePtr stmt)
  {
    addSon(expr);
    addSon(stmt);
  }

  virtual TypePtr checkType() 
  {
    if (sons[0]->checkType()->isCompatible(TR.getStdType(TYPE_BOOL)))
    {
      if (!sons[1]->checkType()->isError())
      {
        typeRec = TR.getStdType(TYPE_OK);
        return typeRec;
      }
    }

    //raise error
    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    typeRec = TR.getErr();
    return typeRec;
  }

  virtual void generateCode() 
  {
    ICodeNode loopBegin = IE.generateJumpTarget();
    ICodeNode loopEnd = IE.generateJumpTarget();

    //the label to the start of the loop
    IE.addJumpTarget(loopBegin);
    IE.pushLoop(IE.getCurrentInstr(), loopEnd);

    sons[1]->generateCode();

    Operand op = sons[0]->evaluteExpr();

    IE.addInstr(OP_JNZ);
    IE.addOperand(op);
    IE.addOperand(loopBegin.jumpTarget);

    IE.addJumpTarget(loopEnd);
    IE.popLoop();    
  }
};

class ForNode : public SyntaxNode
{
public:
  ForNode(NodePtr expr_init, NodePtr expr_end, NodePtr expr_incr, NodePtr stmt)
  {
    addSon(expr_init);
    addSon(expr_end);
    addSon(expr_incr);
    addSon(stmt);
  }

  virtual TypePtr checkType() 
  {
    for (int i = 0; i < 4; ++i)
      if (sons[i]->checkType()->isError())
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        typeRec = TR.getErr();
        return typeRec;
      }

    typeRec =  TR.getStdType(TYPE_OK);
    return typeRec;
  }

  virtual void generateCode() 
  {
    ICodeNode loopBegin = IE.generateJumpTarget();
    ICodeNode loopEnd = IE.generateJumpTarget();

    //the initialization
    sons[0]->evaluteExpr();

    //the label to the start of the loop
    IE.addJumpTarget(loopBegin);
    IE.pushLoop(IE.getCurrentInstr(), loopEnd);

    Operand op = sons[1]->evaluteExpr();

    IE.addInstr(OP_JZ);
    IE.addOperand(op);
    IE.addOperand(loopEnd.jumpTarget);

    //the loop body
    sons[3]->generateCode();

    //the increment statements
    sons[2]->generateCode();
    IE.addInstr(OP_JMP);
    IE.addOperand(loopBegin.jumpTarget);

    IE.addJumpTarget(loopEnd);
    IE.popLoop();
  }
};

class ReturnNode : public SyntaxNode
{
public:
  ReturnNode(NodePtr expr, SymPtr func)
  { 
    symbol = func;
    addSon(expr); 
  }

  ReturnNode(SymPtr func)
  { 
    symbol = func;
  }

  virtual TypePtr checkType() 
  {
    assert(symbol->type->typeID == TYPE_PROD);
    typeRec = symbol->type->getRetType();

    if (sons.size())
    {
      TypePtr retType = sons[0]->checkType();
      if (typeRec->isCompatible(retType))
      {

        if (!typeRec->isEqual(retType))
        {
          NodePtr conversionNode = new CastNode(typeRec, sons[0]);
          conversionNode->checkType();
          sons[0] = conversionNode;
        }
      }
      else
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        typeRec = TR.getErr();
      }
    }
    else
    {
      if (!typeRec->isEqual(TR.getStdType(TYPE_VOID)))
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        typeRec = TR.getErr();
      }
    }
    return typeRec;
  }

  virtual void generateCode() 
  {
    if (sons.size())
    {
      Operand op = sons[0]->evaluteExpr();
      Operand ret;
      ret.type_id = OP_TYPE_REG;
      ret.regCode = REG_RET;
      //puo' essere anche OP_MOV
      xasm(OP_SET, ret, op);
    }
    //RA and (eventually) this
    //xasm(OP_RET, (symbol->getParamCount() + (symbol->isMember ? 2 : 1)));
    //for now this is ALWAYS on stack
    xasm(OP_RET, (symbol->getParamCount() + 2));
  }
};

//
// Expressions
/////////////////////////////////////////////////////

class ExprNode : public SyntaxExprNode
{
public: 
  ExprNode() { }
  ExprNode(NodePtr expr) { addSon(expr); }
  ExprNode(NodePtr expr, NodePtr assignment_expr) 
  { 
    addSon(expr); 
    addSon(assignment_expr);
  }

  virtual bool isLeaf() { return (sons.size() == 0 ? true : false); }
  virtual TypePtr checkType() 
  {
    if (sons.size() == 0)
    {
      typeRec = TR.getStdType(TYPE_OK);
      return typeRec;
    }
    else if (sons.size() == 1)
    {
      typeRec = sons[0]->checkType();
      return typeRec;
    }
    else
    {
      if (!(sons[0]->checkType()->isError()) && !(sons[1]->checkType()->isError()) )
      {
        typeRec = TR.getStdType(TYPE_OK);
        return typeRec;
      }
      else
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        typeRec =  TR.getErr();
        return typeRec;
      }
    }    
  }

  virtual Operand evaluteExpr() 
  {
    if (sons.size() == 0)
      return Operand();
    else if (sons.size() == 1)
      return sons[0]->evaluteExpr();
    else
    {
      sons[0]->evaluteExpr();
      return sons[1]->evaluteExpr();
    } 
  }
};

class IdentUseNode : public SyntaxExprNode
{
private:
  std::string symbolName;
  SymPtr scopeSymbol;

  std::vector<TypePtr> paramTypes;
  bool isFunction;

public:
  IdentUseNode(SymPtr scope_symbol, const std::string& symbol_name) 
  {
    symbolName = symbol_name;
    scopeSymbol = scope_symbol;
    isFunction = false;
  }

  IdentUseNode(SymPtr scope_symbol, const std::string& symbol_name, NodePtr scope_expr) 
  {
    //this is the use of a (b.bb) type expression
    symbolName = symbol_name;
    scopeSymbol = scope_symbol;
    addSon(scope_expr);
    //to get access to a member, it isn't a function!
    //we have to use LEA
    isFunction = false;
  }

  virtual TypePtr checkType() 
  {
    //resolve 
    //e' qui, al tempo del type check, che viene risolto il simbolo!
    if (sons.size())
    {
      sons[0]->checkType();
      
      SymPtr scopeSearchSymbol;
      if (sons[0]->symbol->symKind == SYM_FUNCTION)
        scopeSearchSymbol = sons[0]->typeRec->getSymbol();
      else 
        scopeSearchSymbol = sons[0]->symbol;

      if (isFunction)
        symbol = scopeSearchSymbol->bakerDitchfield(symbolName, paramTypes);
      else
      {
        symbol = scopeSearchSymbol->resolveSymbol(symbolName, paramTypes);
      }
      assert(symbol);
    }
    else
    {
      if (isFunction)
        symbol = scopeSymbol->bakerDitchfield(symbolName, paramTypes);
      else
        symbol = scopeSymbol->resolveSymbol(symbolName, paramTypes);
    }
    if (!symbol)
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      typeRec = TR.getErr();
    }
    else 
      typeRec = symbol->type;  

    return typeRec;
  }

  virtual Operand evaluteExpr() 
  { 
    //controlla di non essere una funzione
    if (typeRec->typeID != TYPE_PROD)
    {
      //our registrer to hold temp pointer values
      Operand regD;
      regD.regCode = REG_D;
      regD.type_id = OP_TYPE_REG;

      //we have to add instructions to store in the 
      //operand with the correct address (at RUN TIME)
      if (sons.size() == 0)
      {
        //sono al punto terminale della catena
        if ((symbol->isArgument) || (symbol->isLocal))
        {
          //sta sullo stack
          //ci penseremo nel passaggio I-Code -> codice
          
          if (symbol->type->typeID == TYPE_CLASS)
          {
            //xasm(OP_LEA, regD, symbol);
            xasm(OP_SET, regD, symbol);
            return (regD);
          }
          return Operand(symbol);
        }
        else if (symbol->isGlobal())
        {
          //sta sullo stack
          //ci penseremo nel passaggio I-Code -> codice
          
          if (symbol->type->typeID == TYPE_CLASS)
          {
            //xasm(OP_LEA, regD, symbol);
            xasm(OP_SET, regD, symbol);
            return (regD);
          }
          return Operand(symbol);

        }
        else //if (symbol->isMember)
        {
          assert(!(symbol->isArgument));
          assert(!(symbol->isLocal));

          //recupera this
          //PUSH REG_A
          //IE.addInstr(OP_PUSH);
          
          //IE.addOperand(regA);
          
          //se e' un membro, so per certo (visto che non ha figli)
          //che e' una costruzione del tipo b = 0; oppure b.bb.s()...
          //comunque e' l'inizio della catena, quindi devo prendere
          //this e ritornarlo, + l'offset di b dalla classe in cui
          //siamo
          
          Operand This;
          This.relStackIndex = STACK_THIS_OFFSET;
          This.type_id = OP_TYPE_REL_STACK_INDEX;
          //LEA REG_D, this
          xasm(OP_LEA, regD, This);

          //trova l'offset del simbolo in symbol
          //lo aggiunge a REG_D (che adesso contiene l'indrizzo
          //di this)
          xasm(OP_PADD, regD, regD, symbol->getClassOffset());

          //cosi', basta che tutte le operazioni della macchina
          //virtuale sappiano trattare coi puntatori (facendo il
          //dereferencing). Magari nei vari metodi retrieveOperandAs?

          return regD;
        }
      }
      else
      {
        //non sono il primo, ricevo gia' un idirizzo
        Operand op = sons[0]->evaluteExpr();

        //TODO: guarda se e' ok anche con piu' di un salto
        xasm(OP_LEA, regD, op);

        //PADD op, op, symbol->varIndex;
        xasm(OP_PADD, regD, regD, Operand(symbol->getClassOffset()));
        return regD;
      }

      //quindi il meccanismo di funzionamento e':
      //trova this
      //LEA REG_B, this (op?) (currentScope?)
      //PADD REG_B, REG_B, offset del simbolo da this
    }
    else
    //l'ident e' una funzione
    {
      Operand regD;
      regD.regCode = REG_D;
      regD.type_id = OP_TYPE_REG;

      //meglio che il FunctionCallNode chiami prima questo e POI generi il codice
      //perche' cosi' riesco a risolvere lo scope (per fare il 
      //PUSH di this!)

      if (sons.size() == 0)
      {
        //NOTE: non dovrebbe essere necessario
        //tutti fanno comunque il push di This
        if ((scopeSymbol->parent->symKind == SYM_CLASS && scopeSymbol->parent->classFlavour == CLASS_CLASS) ||
            (symbol->parent->symKind == SYM_CLASS && symbol->parent->classFlavour == CLASS_CLASS))
        
        {
          //e' una chiamata di funzione in prima posizione: 
          //PUO' ritornare un lhs: esempio: a().b

          Operand This;
          This.relStackIndex = STACK_THIS_OFFSET;
          This.type_id = OP_TYPE_REL_STACK_INDEX;

          //LEA REG_D, this
          xasm(OP_CLONE, regD, This);
        }
        //questo e' il PUSH di this, fatto subito dopo
        //il push degli altri parametri
        else
        {
          Operand op;
          op.type_id = OP_TYPE_OBJECT;
          op.objAddr = (Object*)BAD_PTR;
          xasm(OP_SET, regD, op);
        }

      }
      else
      {
        Operand op = sons[0]->evaluteExpr();

        //chiamata di funzione con scope, esempio: a.b()
        //l'indirizzo di 'a' ci arriva in op;
        
        //in questo caso, il 'this' all'interno del corpo di b
        //e' proprio l'oggetto 'a'
        xasm(OP_CLONE, regD, op);
      }

      //lo facciamo cmq, ci serve un DUMMY
      xasm(OP_PUSH, regD); 

      //cosa deve ritornare? per proseguire (eventualmente) una catena?
      //che fa il type checker?
      return regD;
    }
  }

  void notify(const std::vector<TypePtr>& param_types)
  {
    isFunction = true;
    paramTypes = param_types;
  }
};

class ThisNode : public SyntaxExprNode
{
public:
  ThisNode(SymPtr currentClass)
  {
    symbol = currentClass;
  }

  virtual TypePtr checkType() 
  {
    return symbol->type;
  }

  virtual Operand evaluteExpr() 
  {
    //IE.addInstr(OP_PUSH);
    Operand regD;
    regD.regCode = REG_D;
    regD.type_id = OP_TYPE_REG;
    //IE.addOperand(regA);
          
          
    Operand This;
    This.relStackIndex = STACK_THIS_OFFSET;
    This.type_id = OP_TYPE_REL_STACK_INDEX;
    //LEA REG_D, this
    xasm(OP_LEA, regD, This);

    return regD;
  }
};

class ArrayUseNode : public SyntaxExprNode
{
public:
  ArrayUseNode(NodePtr lhs_expr, NodePtr idx_expr) 
  {  
    addSon(lhs_expr);
    addSon(idx_expr);
  }

  virtual TypePtr checkType() 
  {
    if (!sons[1]->checkType()->isEqual(TR.getStdType(TYPE_INT)))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
 
      typeRec =  TR.getErr();
      return typeRec;
    }

    typeRec = sons[0]->checkType();
    if (!(typeRec->typeID == TYPE_ARRAY || typeRec->typeID == TYPE_STRING))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      typeRec = TR.getErr();
    }
    else
      typeRec = typeRec->getArrayElemType();

    return typeRec;
  }

  virtual Operand evaluteExpr() 
  { 
   
    Operand op1 = sons[1]->evaluteExpr();
    Operand op0 = sons[0]->evaluteExpr();
   
/*
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_MUL, regA, regE, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FMUL, regA, regE, op2);
      else
        assert(0);
    }
    else
    {
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_MUL, regA, op1, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FMUL, regA, op1, op2);
      else
        assert(0);
    }
    */

    Operand ret;
    ret.type_id = OP_TYPE_REG;
    if (op1.type_id == OP_TYPE_REG && op1.regCode == REG_C)
      ret.regCode = REG_D;
    else
      ret.regCode = REG_C;

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    /*
    if (typeRec->typeID == TYPE_ARRAY)
    {
      //TODO: check access with multi-dim arrays
      xasm(OP_PUSH, regA);
      xasm(OP_LEAA, regA, op0, op1);
      xasm(OP_STORE, ret, regA);
      xasm(OP_POP, regA);
    }
    else if (typeRec->typeID == TYPE_STRING)
    {
      //TODO: e se e' usato come LHS?
      //questo va bene per scrivere
      //string a = b[4];
      //ma non
      //b[2] = "c";
      xasm(OP_STRAT, ret, op0, op1);
    }
    */

    if (typeRec->typeID == TYPE_STRING)
    {
      //TODO: e se e' usato come LHS?
      //questo va bene per scrivere
      //string a = b[4];
      //ma non
      //b[2] = "c";
      xasm(OP_STRAT, ret, op0, op1);
    }
    else
    {
      //TODO: check access with multi-dim arrays
      //FIX: non funziona
      //xasm(OP_PUSH, regA);
      //xasm(OP_LEAA, regA, op0, op1);
      xasm(OP_LEAA, ret, op0, op1);
      //xasm(OP_STORE, ret, regA);
      //xasm(OP_POP, regA);
    }
    

    return ret;
  }
};

class ArgListNode : public SyntaxNode
{
private:
  TypePtr formalType;

public:

  ArgListNode(NodePtr expr)
  {
    addSon(expr);
    formalType = NULL;
  }

  ArgListNode(NodePtr other_args, NodePtr expr)
  {
    addSon(other_args);
    addSon(expr);
    formalType = NULL;
  }

  virtual TypePtr checkType() 
  {
    //nothing to do
    if (sons.size() == 2)
    {
      TypePtr type0 = sons[0]->checkType();
      typeRec = sons[1]->checkType();

      if (type0->isEqual(TR.getErr()))
        typeRec = TR.getErr();

        /*
        if (!type2->isEqual(type1))
        {
          //insert conversion code
          NodePtr conversionNode = new CastNode(type1, sons[1]);
          conversionNode->checkType();
          sons[1] = conversionNode;
        }
        */

      return typeRec;
    }
    else
    {
      typeRec = sons[0]->checkType();
      return typeRec;
    }
  }

  void notifyParams(TypePtr param_types)
  {
    if (param_types->getParamCount() == 0)
    {
      //raise error
      std::string s = "Wrong param count";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
    }
    formalType = param_types->typeList.front();


    if (!typeRec->isEqual(formalType))
    {
      //insert conversion code
      if (sons.size() == 2)
      { 
        NodePtr conversionNode = new CastNode(formalType, sons[1]);
        conversionNode->checkType();
        sons[1] = conversionNode;
      }
      else
      {
        NodePtr conversionNode = new CastNode(formalType, sons[0]);
        conversionNode->checkType();
        sons[0] = conversionNode;
      }
    }

    param_types->typeList.pop_front();

    if (sons.size() == 2)
    {
      ArgListNode* params_node = dynamic_cast<ArgListNode*>(sons[0]);
      params_node->notifyParams(param_types);
    }
  }
  
  virtual void generateCode() 
  {
    assert(formalType);
    Operand op;

    if (sons.size() == 2)
      op = sons[1]->evaluteExpr();
    else
      op = sons[0]->evaluteExpr();

    //e' qui che viene deciso
    //l'ordine dei parametri

    //i parametri hanno ordinali che crescono dall'ultimo
    //al primo. siccome i parametri sono messi sullo stack 
    //dall'ultimo al primo (in questo caso), quello piu' lontano
    //dalla base dello stack (si hanno offset negativi) e' quello
    //con ordinale piu' piccolo. Quindi una cosa del tipo 
    //EBP - ordinale - C non va bene per riferirsi ai parametri
    //bisogna in qualche modo aggiustare questa cosa.
    //-> si fa nella getStackOffset


    //qui avviene la distinzione byval/byref (strings are treated default byref
    //by the VM)
    if (formalType->isReference && formalType->isPrimitive()
      && (formalType->typeID != TYPE_STRING))
    {
      //choose a registry for temp storage
      Operand tmp;
      tmp.type_id = OP_TYPE_REG;
      if (op.type_id == OP_TYPE_REG)
      {
        if (op.regCode == REG_A)
          tmp.regCode = REG_D;
        else
          tmp.regCode = REG_A;
      }
      else
        tmp.regCode = REG_A;

      xasm(OP_LEA, tmp, op);
      xasm(OP_PUSH, tmp);
    }
    else if (formalType->typeID == TYPE_STRING)
    {
      //choose a registry for temp storage
      Operand tmp;
      tmp.type_id = OP_TYPE_REG;
      if (op.type_id == OP_TYPE_REG)
      {
        if (op.regCode == REG_A)
          tmp.regCode = REG_D;
        else
          tmp.regCode = REG_A;
      }
      else
        tmp.regCode = REG_A;

      xasm(OP_DEREF, tmp, op);
      xasm(OP_PUSH, tmp);
    }
    else if (formalType->isPrimitive() /*&& (formalType->typeID != TYPE_STRING)*/)
    {
      //parameters byval, strings byref
      IE.addInstr(OP_PUSH);
      IE.addOperand(op);
    }
    else if (formalType->typeID == TYPE_ARRAY)
    {
      //arrays (always by ref)
      xasm(OP_PUSH, op);
    }
    else
    {
      //objects (always by ref)
      Operand regB;
      regB.type_id = OP_TYPE_REG;
      regB.regCode = REG_B;
      xasm(OP_CLONE, regB, op);
      xasm(OP_PUSH, regB);
    }

    if (sons.size() == 2)
      sons[0]->generateCode();
  }
};

class FunctionCallNode : public SyntaxExprNode
{
public:
  FunctionCallNode(NodePtr funcExpr) 
  { 
    addSon(funcExpr); 
    IdentUseNode* call_node = dynamic_cast<IdentUseNode*>(sons[0]);
    std::vector<TypePtr> param_types;
    call_node->notify(param_types);  
  }

  FunctionCallNode(NodePtr funcExpr, NodePtr args) 
  { 
    addSon(funcExpr); 
    addSon(args); 

    std::vector<TypePtr> param_types;
    //build an args list
    //maybe there's also the need to reverse the arg list?

    //qui viene costruita una lista con i tipi dei parametri ATTUALI!
    NodePtr current_param = sons[1];
    while(current_param->sons.size() == 2)
    {
      current_param->sons[1]->checkType();
      param_types.push_back(current_param->sons[1]->typeRec);
      current_param = current_param->sons[0];
    }
    current_param->sons[0]->checkType();
    param_types.push_back(current_param->sons[0]->typeRec);

    //notify the funcExpr of the args...
    //it will be used for the resolution of the 
    //function name in overload

    //il primo discendete e' di sicuro un IdentUseNode
    //dynamic_cast e notify
    IdentUseNode* call_node = dynamic_cast<IdentUseNode*>(sons[0]);
    call_node->notify(param_types);  
  }

  virtual TypePtr checkType() 
  {
    if (sons.size() == 2)
    {
      if (sons[1]->checkType()->isEqual(TR.getErr()))
        typeRec = TR.getErr();
      else
        typeRec = sons[0]->checkType()->getRetType();
      return typeRec;
    }
    else
    {
      typeRec = sons[0]->checkType()->getRetType();
      symbol = sons[0]->getSymbol();
      return typeRec;
    }
  }

  virtual Operand evaluteExpr()
  {
    //il typeRec dell'IdentUseNode contiene il tipo del 
    //simbolo, quindi il tipo dei parametri FORMALI
    //(typeRec = symbol->type;)
    
    //e' questa che fa il push dei parametri
    if (sons.size() == 2)
    {
      //copy construct
      TypePtr formalParamTypes = new TypeDef(*sons[0]->typeRec);

      //dobbiamo comunicarlo ai parametri ATTUALI      
      //formalParamTypes->getParamCount();
      ArgListNode* params_node = dynamic_cast<ArgListNode*>(sons[1]);
      params_node->notifyParams(formalParamTypes);

      //qui si chiama generateCode() perche' questo e' il metodo
      //implementato da ArgListNode. E' questo che poi si occupa
      //di valutare le espressioni (evaluteExpr()) dei singoli parametri
      //attuali
      sons[1]->generateCode();

      //non ci serve piu', ogni ArgListNode ha il suo tipo
      delete formalParamTypes;
    }

    sons[0]->generateCode();
    SymPtr funcSymbol = sons[0]->symbol;
    if (funcSymbol->type->typeID != TYPE_PROD)
    {
      //raise error
      //il termine non restituisce una funzione
      std::string s = "Term does not evalute to a function";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
    }
    if (funcSymbol->parent->type->typeID  != TYPE_CLASS)
    {
      //raise error
      std::string s = "Function call/def invalid outside a class";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
    }

    //dopo il push, call
    if (funcSymbol->isVirtual)
    {
      IE.addInstr(OP_VCALL);
      //NO. questo va bene per le chiamate normali
      //this e' sullo stack

      //IE.addOperand(funcSymbol->parent);
      Operand funcIndex;
      funcIndex.funcIndex = funcSymbol->vtableIndex;
      funcIndex.type_id = OP_TYPE_FUNC;
      IE.addOperand(funcIndex);
      
      /*
      Operand paramCount;
      paramCount.intLiteral = funcSymbol->getParamCount();
      paramCount.type_id = OP_TYPE_INT;
      IE.addOperand(paramCount);
      */
    }
    else
    {
      IE.addInstr(OP_CALL);
      IE.addOperand(funcSymbol->parent->type);
      assert(funcSymbol->parent == funcSymbol->parent->type->getSymbol());
      Operand funcIndex;
      funcIndex.funcIndex = funcSymbol->functionIndex;
      funcIndex.type_id = OP_TYPE_FUNC;
      IE.addOperand(funcIndex);
    }

    Operand regRet;
    regRet.regCode = REG_RET;
    regRet.type_id = OP_TYPE_REG;
    return regRet;
  }

};

class ResolveScopeNode : public SyntaxNode
{
public:
  ResolveScopeNode(SymPtr resolutionScope, NodePtr scopedExpr, NodePtr expr) 
  {
    symbol = resolutionScope;
  }

  virtual TypePtr checkType() {}
  virtual void generateCode() {}
};

class PostIncrNode : public SyntaxNode
{
public:

  PostIncrNode(NodePtr expr)
  {
    addSon(expr);
  }
  virtual TypePtr checkType() 
  {
    typeRec = sons[0]->checkType();
    //must be integral
    if (!typeRec->isEqual(TR.getStdType(TYPE_INT)))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
      typeRec = TR.getErr();
    }
    return typeRec;
  }

  
  //in caso sia usato come statement
  virtual void generateCode() 
  {
    Operand op = sons[0]->evaluteExpr();
    IE.addInstr(OP_INC);
    IE.addOperand(op);
  }

  //in caso sia usato come espressione
  virtual Operand evaluteExpr()
  {
    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;

    Operand op = sons[0]->evaluteExpr();

    xasm(OP_SET, regA, op);
    xasm(OP_INC, op);    
    return regA;
  }
};

class PreIncrNode : public SyntaxNode
{
public:

  PreIncrNode(NodePtr expr)
  {
    addSon(expr);
  }
  virtual TypePtr checkType() 
  {
    typeRec = sons[0]->checkType();
    //must be integral
    if (!typeRec->isEqual(TR.getStdType(TYPE_INT)))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());
      typeRec = TR.getErr();
    }
    return typeRec;
  }

  virtual void generateCode() 
  {
    evaluteExpr();
  }

  virtual Operand evaluteExpr() 
  {
    Operand op = sons[0]->evaluteExpr();
    IE.addInstr(OP_INC);
    IE.addOperand(op);

    return op;
  }
};

class PostDecrNode : public SyntaxNode
{
public:

  PostDecrNode(NodePtr expr)
  {
    addSon(expr);
  }
  virtual TypePtr checkType() 
  {
    typeRec = sons[0]->checkType();
    //must be integral
    if (!typeRec->isEqual(TR.getStdType(TYPE_INT)))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      typeRec = TR.getErr();
    }
    return typeRec;
  }

  virtual void generateCode() 
  {
    Operand op = sons[0]->evaluteExpr();
    IE.addInstr(OP_DEC);
    IE.addOperand(op);
  }

  //in caso sia usato come espressione
  virtual Operand evaluteExpr()
  {
    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;

    Operand op = sons[0]->evaluteExpr();
    xasm(OP_SET, regA, op);
    xasm(OP_DEC, op);    
    return regA;
  }
};

class PreDecrNode : public SyntaxNode
{
public:

  PreDecrNode(NodePtr expr)
  {
    addSon(expr);
  }

  virtual TypePtr checkType() 
  {
    typeRec = sons[0]->checkType();
    //must be integral
    if (!typeRec->isEqual(TR.getStdType(TYPE_INT)))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      typeRec = TR.getErr();
    }
    return typeRec;
  }

  virtual void generateCode() 
  {
    evaluteExpr();
  }

  virtual Operand evaluteExpr() 
  {
    Operand op = sons[0]->evaluteExpr();
    IE.addInstr(OP_DEC);
    IE.addOperand(op);

    return op;
  }
};

class UnaryOpNode : public SyntaxExprNode
{
public:
  enum UN_OP_TYPE
  {
    UN_OP_PLUS,
    UN_OP_MINUS,
    UN_OP_NOT
  };

  UnaryOpNode(UN_OP_TYPE op, NodePtr expr) : opType(op)
  {
    addSon(expr);
  }

  virtual TypePtr checkType() 
  {
    typeRec = sons[0]->checkType();
    switch (opType)
    {
    case UN_OP_PLUS:
    case UN_OP_MINUS:      
      if (typeRec->isEqual(TR.getStdType(TYPE_INT))   ||
          typeRec->isEqual(TR.getStdType(TYPE_FLOAT)))
        return typeRec;
      else
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        return TR.getErr();
      }
      break;
    case UN_OP_NOT:
      if (typeRec->isEqual(TR.getStdType(TYPE_INT))   ||
          typeRec->isEqual(TR.getStdType(TYPE_BOOL)))
        return typeRec;
      else
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        return TR.getErr();
      }
      break;
    default:
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        return TR.getErr();
      }
      break;
    } 
  }

  virtual Operand evaluteExpr()
  {
    Operand op = sons[0]->evaluteExpr();
    switch (opType)
    {
    case UN_OP_PLUS:
      return op;

    case UN_OP_MINUS:
      {
        if (typeRec->typeID == TYPE_INT)
          IE.addInstr(OP_SUB);
        else if (typeRec->typeID == TYPE_FLOAT)
          IE.addInstr(OP_FSUB);
        else
          assert(0);

        IE.addRegOperand(REG_A);
        IE.addOperand(0);
        IE.addOperand(op);
      }
      break;

    case UN_OP_NOT:
      {
        if ((typeRec->typeID == TYPE_INT) || (typeRec->typeID == TYPE_BOOL))
          IE.addInstr(OP_NEG);
        else 
          assert(0);

        IE.addRegOperand(REG_A);
        IE.addOperand(op);
      }
      break;
    }

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }

private:
  UN_OP_TYPE opType;
};

class NewNode : public SyntaxExprNode
{
public:

  NewNode(TypePtr new_type, NodePtr expr) : newType(new_type)
  {
    addSon(expr);
  }

  NewNode(TypePtr new_type) : newType(new_type) { }

  virtual TypePtr checkType() 
  {
    if (sons.size() == 1)
    {
      if (sons[0]->checkType()->isError())
      {
        //raise error
        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());

        typeRec = TR.getErr();
        return typeRec;
      }
    }

    typeRec = newType;
    return typeRec;
  }

  virtual Operand evaluteExpr()
  { 
    Operand regC;
    regC.regCode = REG_C;
    regC.type_id = OP_TYPE_REG;

    //SyntaxTree::GetSingleton().generateNew(symbol->type);
    SyntaxTree::GetSingleton().generateNew(newType);


    if (sons.size() == 1)
    {
      //we have to call the constructor
      assert(newType->typeID == TYPE_CLASS);
      if (newType->typeID != TYPE_CLASS)
      {
        //solo le classi possono avere i costruttori
        //raise error
        std::string s = "Only classes can have constructors";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());
      }

      //TODO2: trova il costruttore giusto e chiamalo
      //newType->getSymbol()...
      xasm(OP_PUSH, regC);
      sons[0]->generateCode();
      xasm(OP_POP, regC);
    }
    return regC;    
  }
  
private:
  TypePtr newType;
};

class NewArrayNode : public SyntaxExprNode
{
public:

  NewArrayNode(TypePtr new_type, NodePtr size_expr) 
    : newType(new_type) { addSon(size_expr); }

  virtual TypePtr checkType() 
  {
    if (!sons[0]->checkType()->isEqual(TR.getStdType(TYPE_INT)))
    {
      //raise error
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      typeRec = TR.getErr();
      return typeRec;
    }

    typeRec = newType;
    return newType;
  }

  virtual Operand evaluteExpr()
  {
    Operand op = sons[0]->evaluteExpr();

    Operand regC;
    regC.regCode = REG_C;
    regC.type_id = OP_TYPE_REG;

    SyntaxTree::GetSingleton().generateNewArray(typeRec, op);

    return regC;
  }
  
private:
  TypePtr newType;
  int dim;
};


class ArithOpNode : public SyntaxExprNode
{
public:
  virtual TypePtr checkType() 
  {
    TypePtr type1 = sons[0]->checkType();
    TypePtr type2 = sons[1]->checkType();

    if ( (!type1->isError()) && !(type2->isError()) )
    {
      if (type1->isCompatible(type2) || type2->isCompatible(type1))
      {
        if (!type1->isEqual(type2))
        {
          if (type1->getCost(type2) < type2->getCost(type1))
          {
            NodePtr conversionNode = new CastNode(type2, sons[0]);
            conversionNode->checkType();
            sons[0] = conversionNode;
            typeRec = type2;
          }
          else
          {
            NodePtr conversionNode = new CastNode(type1, sons[1]);
            conversionNode->checkType();
            sons[1] = conversionNode;
            typeRec = type1;
          }
        }
        else
        {
          typeRec = type1; //doesn't matter          
        }

        return typeRec;
      }
    }

    //raise error
    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    typeRec = TR.getErr();
    return typeRec;    
  }
};

class MulNode : public ArithOpNode
{
public:
  MulNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }  

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_MUL, regA, regE, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FMUL, regA, regE, op2);
      else
        assert(0);
    }
    else
    {
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_MUL, regA, op1, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FMUL, regA, op1, op2);
      else
        assert(0);
    }

    return regA;   
  }
};

class DivNode : public ArithOpNode
{
public:
  DivNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    
    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);

      if (typeRec->typeID == TYPE_INT)
        xasm(OP_DIV, regA, regE, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FDIV, regA, regE, op2);
      else
        assert(0);
    }
    else
    {
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_DIV, regA, op1, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FDIV, regA, op1, op2);
      else
        assert(0);
    }

    return regA;   
  }
};

class AddNode : public ArithOpNode
{
public:
  AddNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);

      if (typeRec->typeID == TYPE_INT)
        xasm(OP_ADD, regA, regE, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FADD, regA, regE, op2);
      else
        assert(0);
    }
    else
    {
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_ADD, regA, op1, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FADD, regA, op1, op2);
      else
        assert(0);
    }

    return regA;   
  }
};

class SubNode : public ArithOpNode
{
public:
  SubNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);

      if (typeRec->typeID == TYPE_INT)
        xasm(OP_SUB, regA, regE, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FSUB, regA, regE, op2);
      else
        assert(0);
    }
    else
    {
      if (typeRec->typeID == TYPE_INT)
        xasm(OP_SUB, regA, op1, op2);
      else if (typeRec->typeID == TYPE_FLOAT)
        xasm(OP_FSUB, regA, op1, op2);
      else
        assert(0);
    }

    return regA;   
  }
};

class IntegralOpNode : public SyntaxExprNode
{
public:
  virtual TypePtr checkType() 
  {
    TypePtr type1 = sons[0]->checkType();
    TypePtr type2 = sons[1]->checkType();

    if ( type1->isCompatible(TR.getStdType(TYPE_INT)) && type2->isCompatible(TR.getStdType(TYPE_INT)) )
    {
      if (type1->typeID != TYPE_INT)
      {
        NodePtr conversionNode = new CastNode(TR.getStdType(TYPE_INT), sons[0]);
        conversionNode->checkType();
        sons[0] = conversionNode;
      }

      if (type2->typeID != TYPE_INT)
      {
        NodePtr conversionNode = new CastNode(TR.getStdType(TYPE_INT), sons[1]);
        conversionNode->checkType();
        sons[1] = conversionNode;
      }

      typeRec = TR.getStdType(TYPE_INT);
      return typeRec;
    }
    else
    {
      //raise error
      typeRec = TR.getErr();
 
      std::string s = "Type error";
      ErrorManager::GetSingleton().logAndRaise(s.c_str());

      return typeRec;    
    }
  }
};

class ModNode : public IntegralOpNode
{
public:
  ModNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      xasm(OP_MOD, regA, regE, op2);
    }
    else
    {
      xasm(OP_MOD, regA, op1, op2);
    }

    return regA;   
  }
};

class LshNode : public IntegralOpNode
{
public:
  LshNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      xasm(OP_LSH, regA, regE, op2);
    }
    else
    {
      xasm(OP_LSH, regA, op1, op2);        
    }

    return regA;   
  }
};

class RshNode : public IntegralOpNode
{
public:
  RshNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      xasm(OP_RSH, regA, regE, op2);
    }
    else
    {
      xasm(OP_RSH, regA, op1, op2);
    }

    return regA;   
  }
};

class ComparisonNode : public SyntaxExprNode
{
public:
  virtual TypePtr checkType() 
  {
    TypePtr type1 = sons[0]->checkType();
    TypePtr type2 = sons[1]->checkType();

    if ( (!type1->isError()) && !(type2->isError()) )
    {
      if (type1->isCompatible(type2) || type2->isCompatible(type1))
      {
        if (!type1->isEqual(type2))
        {
          if (type1->getCost(type2) < type2->getCost(type1))
          {
            NodePtr conversionNode = new CastNode(type2, sons[0]);
            conversionNode->checkType();
            sons[0] = conversionNode;
          }
          else
          {
            NodePtr conversionNode = new CastNode(type1, sons[1]);
            conversionNode->checkType();
            sons[1] = conversionNode;
          }
        }
      }
     
      typeRec = TR.getStdType(TYPE_BOOL);
      return typeRec;
    }

    //raise error
    typeRec = TR.getErr();

    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    return typeRec;    
  }
};

class LessNode : public ComparisonNode
{
public:
  LessNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }


  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    Operand op2 = sons[1]->evaluteExpr();

    ICodeNode trueTarget = IE.generateJumpTarget();
    ICodeNode exitTarget = IE.generateJumpTarget();

    IE.addInstr(OP_JL);
    
    IE.addOperand(op1);
    IE.addOperand(op2);
    IE.addOperand(trueTarget.jumpTarget);
    
    //in case of false
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(false);

    IE.addInstr(OP_JMP);
    IE.addOperand(exitTarget.jumpTarget);

    //in case of true
    IE.addJumpTarget(trueTarget);
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(true);

    IE.addJumpTarget(exitTarget);

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }
};

class GreaterNode : public ComparisonNode
{
public:
  GreaterNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    Operand op2 = sons[1]->evaluteExpr();

    ICodeNode trueTarget = IE.generateJumpTarget();
    ICodeNode exitTarget = IE.generateJumpTarget();

    IE.addInstr(OP_JG);
   
    IE.addOperand(op1);
    IE.addOperand(op2);
    IE.addOperand(trueTarget.jumpTarget);
    
    //in case of false
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(false);

    IE.addInstr(OP_JMP);
    IE.addOperand(exitTarget.jumpTarget);

    //in case of true
    IE.addJumpTarget(trueTarget);
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(true);

    IE.addJumpTarget(exitTarget);

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }
};

class LENode : public ComparisonNode
{
public:
  LENode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    Operand op2 = sons[1]->evaluteExpr();

    ICodeNode trueTarget = IE.generateJumpTarget();
    ICodeNode exitTarget = IE.generateJumpTarget();

    IE.addInstr(OP_JLE);
    IE.addOperand(op1);
    IE.addOperand(op2);
    IE.addOperand(trueTarget.jumpTarget);
    
    //in case of false
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(false);

    IE.addInstr(OP_JMP);
    IE.addOperand(exitTarget.jumpTarget);

    //in case of true
    IE.addJumpTarget(trueTarget);
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(true);

    IE.addJumpTarget(exitTarget);

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }
};

class GENode : public ComparisonNode
{
public:
  GENode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    Operand op2 = sons[1]->evaluteExpr();

    ICodeNode trueTarget = IE.generateJumpTarget();
    ICodeNode exitTarget = IE.generateJumpTarget();

    IE.addInstr(OP_JGE);
    IE.addOperand(op1);
    IE.addOperand(op2);
    IE.addOperand(trueTarget.jumpTarget);
    
    //in case of false
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(false);

    IE.addInstr(OP_JMP);
    IE.addOperand(exitTarget.jumpTarget);

    //in case of true
    IE.addJumpTarget(trueTarget);
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(true);

    IE.addJumpTarget(exitTarget);

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }
};

class EqualNode : public ComparisonNode
{
public:
  EqualNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    Operand op2 = sons[1]->evaluteExpr();

    ICodeNode trueTarget = IE.generateJumpTarget();
    ICodeNode exitTarget = IE.generateJumpTarget();

    IE.addInstr(OP_JE);

    IE.addOperand(op1);
    IE.addOperand(op2);
    IE.addOperand(trueTarget.jumpTarget);
    
    //in case of false
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(false);

    IE.addInstr(OP_JMP);
    IE.addOperand(exitTarget.jumpTarget);

    //in case of true
    IE.addJumpTarget(trueTarget);
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(true);

    IE.addJumpTarget(exitTarget);

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }
};

class NEqualNode : public ComparisonNode
{
public:
  NEqualNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual void generateCode() {}
  
  virtual Operand evaluteExpr()
  {
    Operand op1 = sons[0]->evaluteExpr();
    Operand op2 = sons[1]->evaluteExpr();

    ICodeNode trueTarget = IE.generateJumpTarget();
    ICodeNode exitTarget = IE.generateJumpTarget();

    IE.addInstr(OP_JNE);
    
    IE.addOperand(op1);
    IE.addOperand(op2);
    IE.addOperand(trueTarget.jumpTarget);
    
    //in case of false
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(false);

    IE.addInstr(OP_JMP);
    IE.addOperand(exitTarget.jumpTarget);

    //in case of true
    IE.addJumpTarget(trueTarget);
    IE.addInstr(OP_SET);
    IE.addRegOperand(REG_A);
    IE.addOperand(true);

    IE.addJumpTarget(exitTarget);

    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;
    return regA;
  }

};

class BitAndNode : public IntegralOpNode
{
public:
  BitAndNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    if (!typeRec->typeID == TYPE_INT)
      assert(0);

    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      xasm(OP_AND, regA, regE, op2);
    }
    else
    {
      xasm(OP_AND, regA, op1, op2);
    }

    return regA;   
  }
};

class BitOrNode : public IntegralOpNode
{
public:
  BitOrNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    if (!typeRec->typeID == TYPE_INT)
      assert(0);

    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {    
      xasm(OP_POP, regE);
      xasm(OP_OR, regA, regE, op2);
    }
    else
      xasm(OP_OR, regA, op1, op2);

    return regA;   
  }
};

class BitXorNode : public IntegralOpNode
{
public:
  BitXorNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    if (!typeRec->typeID == TYPE_INT)
      assert(0);

    Operand op1 = sons[0]->evaluteExpr();
    if (op1.type_id == OP_TYPE_REG)
      xasm(OP_PUSH, op1);

    Operand op2 = sons[1]->evaluteExpr();

    Operand regA;
    regA.type_id = OP_TYPE_REG;
    regA.regCode = REG_A;

    Operand regE;
    regE.type_id = OP_TYPE_REG;
    regE.regCode = REG_E;

    if (op1.type_id == OP_TYPE_REG)
    {
      xasm(OP_POP, regE);
      xasm(OP_XOR, regA, regE, op2);
    }
    else
      xasm(OP_XOR, regA, op1, op2);

    return regA;   
  }
};

class AndNode : public IntegralOpNode //ha il checkType giusto
{
public:
  AndNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    ICodeNode lastInstr = IE.generateJumpTarget();

    assert(typeRec->isCompatible(TR.getStdType(TYPE_INT)));
    
    Operand op1 = sons[0]->evaluteExpr();
    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;

    xasm(OP_SET, regA, op1);
    xasm(OP_JZ, regA, lastInstr.jumpTarget);

    Operand op2 = sons[1]->evaluteExpr();

    xasm(OP_SET, regA, op2);   
    IE.addJumpTarget(lastInstr);

    return regA;   
  }
};

class OrNode : public IntegralOpNode //ha il checkType giusto
{
public:
  OrNode(NodePtr left, NodePtr right)
  {
    addSon(left);
    addSon(right);
  }

  virtual Operand evaluteExpr()
  {
    ICodeNode lastInstr = IE.generateJumpTarget();

    assert(typeRec->isCompatible(TR.getStdType(TYPE_INT)));
    
    Operand op1 = sons[0]->evaluteExpr();
    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;

    xasm(OP_SET, regA, op1);
    xasm(OP_JNZ, regA, lastInstr.jumpTarget);

    Operand op2 = sons[1]->evaluteExpr();

    xasm(OP_SET, regA, op2);   
    IE.addJumpTarget(lastInstr);

    return regA;
  }
};

class ThreeNode : public SyntaxNode
{
public:
  ThreeNode(NodePtr left, NodePtr expr1, NodePtr expr2)
  {
    addSon(left);
    addSon(expr1);
    addSon(expr2);
  }

  virtual TypePtr checkType() 
  {
    TypePtr typeCond = sons[0]->checkType();
    TypePtr type1 = sons[1]->checkType();
    TypePtr type2 = sons[2]->checkType();

    if ( typeCond->isError() || !(typeCond->isCompatible(TR.getStdType(TYPE_BOOL))) )
    {
      //raise error
      typeRec = TR.getErr();
      return typeRec;  
    }
     
    if ( (!type1->isError()) && !(type2->isError()) )
    {
      if (!(type1->isCompatible(type2)) && !(type2->isCompatible(type1)))
      {
        //no conversion possible
        //raise error
        typeRec = TR.getErr();

        std::string s = "Type error";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());
      }
      else
      {
        //scegli il migliore e converti
        if (!type1->isEqual(type2))
        {
          //typeRec = type1->mergeType(type2);
          if (type1->getCost(type2) < type2->getCost(type1))
          {
            NodePtr conversionNode = new CastNode(type2, sons[0]);
            conversionNode->checkType();
            sons[0] = conversionNode;
            typeRec = type2;
          }
          else
          {
            NodePtr conversionNode = new CastNode(type1, sons[1]);
            conversionNode->checkType();
            sons[1] = conversionNode;
            typeRec = type1;
          }
        }
        else
        {
          typeRec = type1; //doesn't matter          
        }

      }
      return typeRec;
    }

    //raise error
    typeRec = TR.getErr();

    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    return typeRec;     
  }

  virtual Operand evaluteExpr()
  {
    Operand regA;
    regA.regCode = REG_A;
    regA.type_id = OP_TYPE_REG;

    ICodeNode target1 = IE.generateJumpTarget();
    ICodeNode target2 = IE.generateJumpTarget();

    //if
    Operand op1 = sons[0]->evaluteExpr();
    xasm(OP_JZ, op1, target1.jumpTarget);
    //then
    sons[1]->generateCode();
    xasm(OP_SET, regA, 1);
    xasm(OP_JMP, target2.jumpTarget);

    //else
    IE.addJumpTarget(target1);
    sons[2]->generateCode();

    xasm(OP_SET, regA, 0);
    IE.addJumpTarget(target2);   
    return regA;
  }
};

class AssignementNode : public SyntaxNode
{
public:
  enum ASSIGN_TYPE
  {
    ASSIGN,
    ASSIGN_MUL,
	  ASSIGN_DIV,   
	  ASSIGN_MOD,   
	  ASSIGN_ADD,   
    ASSIGN_SUB,
	  ASSIGN_LEFT,
	  ASSIGN_RIGHT,
	  ASSIGN_AND,
	  ASSIGN_XOR,
	  ASSIGN_OR
  };

  AssignementNode(NodePtr lhs, NodePtr rhs, ASSIGN_TYPE op_kind)
    : opKind(op_kind) 
  { 
    addSon(lhs); 
    addSon(rhs);
  }

  virtual TypePtr checkType() 
  {
    TypePtr type1 = sons[0]->checkType();
    TypePtr type2 = sons[1]->checkType();

    if ( (!type1->isError()) && !(type2->isError()) )
      if (type2->isCompatible(type1)) //NB: one way in this case (only type2 -> type1)
      {
        //controlla anche che left side sia assegnabile
        //non const e != string literal
        if (!type2->isEqual(type1))
        {
          //insert conversion code
          NodePtr conversionNode = new CastNode(type1, sons[1]);
          conversionNode->checkType();
          sons[1] = conversionNode;
        }

        typeRec = type1;
        return typeRec;
      }

    //raise error
    typeRec = TR.getErr();

    std::string s = "Type error";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    return typeRec;    
  }

  virtual void generateCode() 
  {
    //it can be a statement on his own
    evaluteExpr();
  }

  virtual Operand evaluteExpr()
  {
    Operand op2 = sons[1]->evaluteExpr();
    Operand op1 = sons[0]->evaluteExpr();

    if (opKind == ASSIGN)
    {
      xasm(OP_MOV, op1, op2);
      return op1;
    }
    else
    {
      switch (opKind)
      {
      case ASSIGN_MUL:
        if (typeRec->typeID == TYPE_INT)
          IE.addInstr(OP_MUL);
        else if (typeRec->typeID  == TYPE_FLOAT)
          IE.addInstr(OP_FMUL);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?  
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_DIV:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_DIV);
        else if (typeRec->typeID  == TYPE_FLOAT)
          IE.addInstr(OP_FDIV);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_MOD: 
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_MUL);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_ADD:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_ADD);
        else if (typeRec->typeID  == TYPE_FLOAT)
          IE.addInstr(OP_FADD);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_SUB:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_SUB);
        else if (typeRec->typeID  == TYPE_FLOAT)
          IE.addInstr(OP_FSUB);
        else 
         {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_LEFT:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_LSH);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_RIGHT:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_RSH);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_AND:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_AND);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_XOR:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_XOR);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      case ASSIGN_OR:
        if (typeRec->typeID  == TYPE_INT)
          IE.addInstr(OP_OR);
        else 
        {
          //raise error
          //come e' possibile essere arrivati qui?      
          std::string s = "Type error";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());

          assert(0);
        }
        break;

      }
      //il risultato in EAX
      IE.addRegOperand(REG_A);

      IE.addOperand(op1);
      IE.addOperand(op2);

      IE.addInstr(OP_MOV);      
      IE.addOperand(op1);
      IE.addRegOperand(REG_A);
      return op1;
    }
  }

private:
  ASSIGN_TYPE opKind;

};


//
// Utilities
/////////////////////////////////////////////////////

class EmptyNode : public ISyntaxNode
{
public:
  virtual bool isLeaf() { return true; }
  virtual TypePtr checkType() { return TypeRoot::GetSingleton().getStdType(TYPE_OK); }
  virtual void generateCode() { }
  virtual Operand evaluteExpr() { return Operand(); } 
};




#endif //SYNTAX_TREE_H_INCLUDED_


