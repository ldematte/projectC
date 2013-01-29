%{


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <new>

#include <common.h>
#include "type.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "SyntaxTree.h"
#include "ScopeManager.h"
#include "OutputManager.h"
#include "ErrorManager.h"

#include "lexer.h"

#define scope_stack  ScopeManager::GetSingleton()
#define type_tree    TypeRoot::GetSingleton()
#define symbol_table SymbolTable::GetSingleton()
#define parse_tree   SyntaxTree::GetSingleton()
#define output       OutputManager::GetSingleton()

#define YYDEBUG 1
#define YYERROR_VERBOSE

%}

%union {
  TypePtr  type;
  NodePtr  node;
  char*    name;
  float    realval;
  int      intval;
  bool     boolval;
}

///I nostri vari token

%token <intval> IDENTIFIER TYPENAME
%token <realval>  FLOAT_CONSTANT 
%token <intval>  INT_CONSTANT
%token <name>    STRING_LITERAL 
%token <boolval> TRUE FALSE
%token SIZEOF THIS
%token INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN

%token TYPEDEF STATIC VIRTUAL PUBLIC PRIVATE PROTECTED NATIVE
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT CONST BOOL VOID STRING 
%token STRUCT CLASS UNION ENUM 
%token NEW

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR EACH IN CONTINUE BREAK RETURN

%type <node> declaration_block translation_unit
%type <node> function_definition declaration class_declaration
%type <node> class_body

%type <type> type_decl type_specifier
%type <boolval> arg_modifier
%type <intval> access_modifier

%type <node> identifier_list declarator init_declarator func_declarator
%type <node> initializer

%type <type> parameter_type_list parameter_decl_clause

%type <node> statement labeled_statement compound_statement expression_statement
%type <node> selection_statement iteration_statement jump_statement
%type <node> statement_list 

%type <node> primary_expression expression postfix_expression
%type <node> argument_expression_list unary_expression cast_expression 
%type <node> new_expression multiplicative_expression additive_expression
%type <node> shift_expression relational_expression equality_expression
%type <node> and_expression exclusive_or_expression inclusive_or_expression
%type <node> logical_and_expression logical_or_expression for_init_statement
%type <node> conditional_expression assignment_expression 

%type <node> constant_expression

%type <intval> unary_operator assignment_operator


%start translation_unit
%%


///il nostro file e' una serie di blocchi di dichiarazione
translation_unit
  : declaration_block                  { parse_tree.addSon(new DeclBlock($1)); } 
  | translation_unit declaration_block { parse_tree.addSon(new DeclBlock($2)); }
  ;
  
///i blocchi di dichiarazione sono dichiarazioni base (id, funzioni)
///o definizioni di funzioni -- o classi!
declaration_block
  : function_definition { $$ = $1; }
  | declaration         { $$ = $1; }
  | class_declaration   { $$ = $1; }
  ;
  
  
class_body
  : /*empty*/  { $$ = new EmptyNode(); }
  | access_modifier class_body
    { 
      scope_stack.getCurrentScope()->accessLevel = (ACCESS_LEVEL)$1;
      $$ = $2;
    }         
  | declaration_block class_body
    {
      $$ = new ClassBodyNode($1, $2);      
    }
  ;

///Ma anche la nostra classe puo' essere intesa come blocchi di 
///dichiarazione!
class_declaration
  : CLASS IDENTIFIER '{' 
    { 
      SymPtr new_class = symbol_table.declareClass(symbol_table.getLexToken($2), 
                                                   scope_stack.getCurrentScope());
      scope_stack.pushScope(new_class); 
    } 
    class_body '}'';' 
    {
      //we have done with the current class
      SymPtr new_class = scope_stack.getCurrentScope();
      //new_class->updateType();
      $$ = new ClassNode($5, new_class);           
      scope_stack.popScope(); 
    }     
  | CLASS IDENTIFIER ':' TYPENAME '{'
    { 
      SymPtr new_class = symbol_table.declareClass(symbol_table.getLexToken($2), 
                                                   symbol_table.getLexToken($4), 
                                                   scope_stack.getCurrentScope());
      scope_stack.pushScope(new_class); 
    } 
    class_body '}' ';' 
    { 
      //we have done with the current class
      SymPtr new_class = scope_stack.getCurrentScope();
      //new_class->updateType(); 
      $$ = new ClassNode($7, new_class);           
      scope_stack.popScope(); 
    } 
  ;
  
///cos'e' una definizione di funzione?
function_definition
  : STATIC type_decl func_declarator '{' 
    {
      SymPtr class_scope = scope_stack.getCurrentScope()->parent;       	  
 	  SymPtr new_func = symbol_table.declareFunction2($3->getSymbol(), $2, 
 	                                                  class_scope->accessLevel);
	                                                 
	  new_func->isStatic = true;
	  new_func->isDefined = true;
   	  scope_stack.pushScope(new_func);	  
    } 
    statement_list '}' 
    {
      $$ = new FunctionDefNode($6, $3->getSymbol());            
      scope_stack.popScope();       
    }
  | VIRTUAL type_decl func_declarator '{'
    { 
      SymPtr class_scope = scope_stack.getCurrentScope()->parent;
      if (class_scope->type->typeID != TYPE_CLASS)
      {
        //raise error
        ErrorManager::GetSingleton().logAndRaise("\"virtual\" allowed only in classes");
      }
	  
      SymPtr new_func = symbol_table.declareVirtualFunction($3->getSymbol(), $2,
	                                                        class_scope->accessLevel);
   	  new_func->isDefined = true;
    } 
    statement_list '}'
    { 
      $$ = new FunctionDefNode($6, $3->getSymbol());
      scope_stack.popScope();      
    }
  | type_decl func_declarator '{'
    { 
      SymPtr class_scope = scope_stack.getCurrentScope()->parent;
      SymPtr new_func = symbol_table.declareFunction2($2->getSymbol(), $1,
	                                                  class_scope->accessLevel);

  	  new_func->isDefined = true;
      new_func->isCtor = (new_func->name == class_scope->name);
    } 
    statement_list '}'
    { 
      $$ = new FunctionDefNode($5, $2->getSymbol());
      scope_stack.popScope();      
    }
    
  | NATIVE type_decl declarator ';'
	  {
	    SymPtr class_scope = scope_stack.getCurrentScope()->parent;       	  
 	    /*SymPtr new_func = */ symbol_table.declareNativeFunction2($3->getSymbol(), $2, 
 	                                                          class_scope->accessLevel);
 	                                                         
 	    $$ = new EmptyNode();
        scope_stack.popScope();     
 	  }
  ;
  
  
type_decl
  : CONST type_specifier  { $$ = TypeDef::createConst($2); }
  | type_specifier        { $$ = $1;          }
  ;

///DICHIARAZIONI
type_specifier
	: VOID       { $$ = type_tree.getStdType(TYPE_VOID);   }
	| CHAR       { $$ = type_tree.getStdType(TYPE_INT);    }
	| SHORT      { $$ = type_tree.getStdType(TYPE_INT);    }
	| INT        { $$ = type_tree.getStdType(TYPE_INT);    }
	| LONG       { $$ = type_tree.getStdType(TYPE_INT);    }
	| FLOAT      { $$ = type_tree.getStdType(TYPE_FLOAT);  }
	| SIGNED     { $$ = type_tree.getStdType(TYPE_INT);    }
	| UNSIGNED   { $$ = type_tree.getStdType(TYPE_INT);    }
    | STRING     { $$ = type_tree.getStdType(TYPE_STRING); }
    | BOOL       { $$ = type_tree.getStdType(TYPE_BOOL);   }
	| TYPENAME   
	  { 
	    SymPtr searchScope = scope_stack.getCurrentScope();
	    if (searchScope->type == NULL)
	      searchScope = searchScope->parent;
	      
	    $$ = searchScope->type->resolveName(symbol_table.getLexToken($1));	   
	  }
	;


access_modifier
  : PUBLIC    { $$ = ACCESS_PUBLIC;    }
  | PRIVATE   { $$ = ACCESS_PRIVATE;   }
  | PROTECTED { $$ = ACCESS_PROTECTED; }
  ;  

arg_modifier
  : /*empty*/ { $$ = true;  }
  | '&'       { $$ = false; }
  ;

parameter_decl_clause
  : /*empty*/           {  $$ = TypeDef::createProduct(); }
  | parameter_type_list {  $$ = $1;                       }
  ;

parameter_type_list
  : type_decl arg_modifier IDENTIFIER 
    { 
	  SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter($1, $2);
      symbol_table.declareParam(symbol_table.getLexToken($3), var, current_scope);
      TypePtr param_list = TypeDef::createProduct();
      param_list->addParam(var);
	  $$ = param_list;
	}
  | type_decl arg_modifier IDENTIFIER  ',' parameter_type_list 
    { 
      SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter($1, $2);
      symbol_table.declareParam(symbol_table.getLexToken($3), var, current_scope);
	  $5->addParam(var);
	  $$ = $5;
    }    
  | type_decl arg_modifier IDENTIFIER '['']'
    { 
	  SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter($1, $2, true);
      symbol_table.declareParam(symbol_table.getLexToken($3), var, current_scope);
      TypePtr param_list = TypeDef::createProduct();
      param_list->addParam(var);
	  $$ = param_list;
	}
  | type_decl arg_modifier IDENTIFIER '['']' ',' parameter_type_list 
    { 
      SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter($1, $2, true);
      symbol_table.declareParam(symbol_table.getLexToken($3), var, current_scope);
	  $7->addParam(var);
	  $$ = $7;
    }
  ;

declaration
  : STATIC type_decl identifier_list ';'
    {
      //a tutti gli id in identifier_list applico le info sul tipo
      NodePtr decl = new DeclListNode($2, $3, true); //type, id list, is static
      decl->propagateTypeInfo(decl->getType(), true);
      $$ = decl;
    }
  | type_decl identifier_list ';'
    {
      //a tutti gli id in identifier_list applico le info sul tipo
      NodePtr decl = new DeclListNode($1, $2, false); //type, id list, is static
      decl->propagateTypeInfo(decl->getType(), false);
      $$ = decl;
    }
  ;

identifier_list
  : init_declarator                      { $$ = new DeclNode($1); }
  | identifier_list ',' init_declarator  { $$ = new DeclNode($3, $1); }
  ;
  
init_declarator 
  : declarator             
    {
      if (!($1->getSymbol()->isFullyDeclared))
        symbol_table.declareVar($1->getSymbol(), TR.getStdType(TYPE_VOID), scope_stack.getCurrentScope());  
      $$ = $1;          
    }
  | declarator initializer 
    { 
      if (!($1->getSymbol()->isFullyDeclared))
        symbol_table.declareVar($1->getSymbol(), TR.getStdType(TYPE_VOID), scope_stack.getCurrentScope()); 
      $$ = new InitDeclNode($1, $2); 
    }
  ;  
  
initializer
  : '=' assignment_expression { $$ = new AssignInitNode($2); }
  /*| '(' expression ')'        { $$ = new InitCtorNode($2); }*/
  ;
  
declarator
	: IDENTIFIER
	  {
	    SymPtr current_scope = scope_stack.getCurrentScope();
	    
	    //we don't know if this is a function or a var, we need
	    //to disambiguate at an higher level
	    //per ora facciamo finta che sia una variabile  semplice
        SymPtr new_var = symbol_table.declareVar(symbol_table.getLexToken($1), current_scope);      
        $$ = new VarDeclNode(new_var);
	  }
	| declarator '[' INT_CONSTANT ']' //e' un array
	  {	    
	    TypePtr var_type = TypeDef::createEmptyArray($3);
	    SymPtr arraySymbol = symbol_table.declareVar($1->getSymbol(), var_type, scope_stack.getCurrentScope());
	    $$ = new VarDeclArrayNode($1, arraySymbol);
      }
	| declarator '[' ']'  //e' un array senza dimensione !funziona per piu' 
                        //dimensioni!
	  {
	    TypePtr var_type = TypeDef::createEmptyArray();
	    SymPtr arraySymbol = symbol_table.declareVar($1->getSymbol(), var_type, scope_stack.getCurrentScope());
	    $$ = new VarDeclArrayNode($1, arraySymbol);
      }
	| func_declarator { $$ = $1; }
	
	;

func_declarator
    : declarator 
      {
        $1->getSymbol()->symKind = SYM_FUNCTION;
	    scope_stack.pushScope($1->getSymbol());
	  }  
	  '(' parameter_decl_clause ')' //dichiarazione di funzione
	  {	    
        SymPtr new_var = symbol_table.declareFunction1($1->getSymbol(), $4);        
        $$ = new FunctionDeclNode(new_var);	    
	  }
    
	;
  
///STATEMENT
statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	| declaration
	;

///a block
compound_statement 
    : '{' '}' { $$ = new EmptyNode(); }
    | '{' 
      { 
        SymPtr block = symbol_table.declareLclBlock(scope_stack.getCurrentScope());
        scope_stack.pushScope(block);
      }  
      statement_list 
      '}'
      {
        $$ = new StatementListNode($3);                
        //finally
        scope_stack.popScope();
      }      
    ;
  
statement_list
	: statement 
	  {
	    $$ = $1;
	  }
	| statement_list statement
	  {
	    $$ = new StatementListNode($1, $2);
	  }
	;



///i vari tipi di statement
labeled_statement
	: CASE constant_expression ':' statement
	  {
	    $$ = new CaseNode($2, $4);
	  }
	| DEFAULT ':' statement
	  {
	    $$ = new CaseNode($3);
	  }
	;

expression_statement //solo per correttezza sintattica
	: ';'
	  {
	    $$ = new ExprNode();
	  }
	| expression ';'  
	  {
	    $$ = $1;
	  } 
	;

selection_statement
	: IF '(' expression ')' statement
	  {
	    $$ = new ITENode($3, $5);
	  }
	| IF '(' expression ')' statement ELSE statement
	  {
	    $$ = new ITENode($3, $5, $7);
	  }
    | SWITCH '(' expression ')' statement
      {
	    $$ = new SwitchNode($3, $5);
	  }
	;
	
for_init
    : FOR '('  
      {
	    SymPtr block = symbol_table.declareLclBlock(scope_stack.getCurrentScope());
        scope_stack.pushScope(block);
	  }	 
    ;

iteration_statement
	: WHILE '(' expression ')' statement
	  {
	    $$ = new WhileNode($3, $5);
	  }
	| DO statement WHILE '(' expression ')' ';'
	  {
	    $$ = new DoWhileNode($5, $2);
	  }
	| for_init for_init_statement expression_statement ')' statement
	  {
	    $$ = new ForNode($2, $3, new ExprNode(), $5);
	    scope_stack.popScope();
	  }
	| for_init for_init_statement expression_statement expression ')' statement
	  {
	    $$ = new ForNode($2, $3, $4, $6);
	    scope_stack.popScope();
	  }
    //| FOR EACH IDENTIFIER IN IDENTIFIER statement///second must be an array of first
	;
	
for_init_statement
    : expression_statement { $$ = $1; }
    | declaration          { $$ = $1; }
    ;

jump_statement
	: CONTINUE ';'           { $$ = new ContinueNode(); }
	| BREAK ';'              { $$ = new BreakNode(); }
	| RETURN ';'             { $$ = new ReturnNode(scope_stack.getCurrentScope()); }
	| RETURN expression ';'  { $$ = new ReturnNode($2, scope_stack.getCurrentScope()); } 
	;


///ESPRESSIONI
primary_expression
	: IDENTIFIER 
	  { 
	    $$ = new IdentUseNode(scope_stack.getCurrentScope(), symbol_table.getLexToken($1)); 
	  }
    | THIS       { $$ = new ThisNode(scope_stack.getCurrentScope()); }
	| INT_CONSTANT   { $$ = new IntNode($1); }
	| FLOAT_CONSTANT { $$ = new FloatNode($1); }
	| STRING_LITERAL { $$ = new StringNode($1); }
	| TRUE           { $$ = new BoolNode(true); }
	| FALSE          { $$ = new BoolNode(false); }
	| '(' expression ')' { $$ = new ExprNode($2); }
	;

postfix_expression
	: primary_expression                    { $$ = $1; }
	| postfix_expression '[' expression ']' { $$ = new ArrayUseNode($1, $3); }
	| postfix_expression '(' ')'            { $$ = new FunctionCallNode($1); }
  | postfix_expression '(' argument_expression_list ')' 
    { 
      $$ = new FunctionCallNode($1, $3); 
    }
  | postfix_expression '.' IDENTIFIER
    {
	    //HERE
  	  $$ = new IdentUseNode(scope_stack.getCurrentScope(), symbol_table.getLexToken($3), $1);
    }
	| postfix_expression INC_OP { $$ = new PostIncrNode($1); }
	| postfix_expression DEC_OP { $$ = new PostDecrNode($1); }
	;

argument_expression_list
	: assignment_expression                              { $$ = new ArgListNode($1); }
	| argument_expression_list ',' assignment_expression { $$ = new ArgListNode($1, $3); }
	;

unary_expression
	: postfix_expression             { $$ = $1 }
	| INC_OP unary_expression        { $$ = new PreIncrNode($2); }
	| DEC_OP unary_expression        { $$ = new PreDecrNode($2); }
	| unary_operator cast_expression { $$ = new UnaryOpNode((UnaryOpNode::UN_OP_TYPE)$1, $2); }
	| SIZEOF '(' type_specifier ')' 
	  { 
	    //TODO2: magari migliorarlo x gli array?
	    //o per le stringhe? in futuro, magari...
	    int dim = $3->getTypeSize();
	    if (dim == 0) 
	    {
	      //raise error
	    } 
	    $$ = new IntNode(dim); 
	  }
	| new_expression                 { $$ = $1; }
	;

unary_operator
	: '+'  { $$ = UnaryOpNode::UN_OP_PLUS;  }
	| '-'  { $$ = UnaryOpNode::UN_OP_MINUS; }
	| '!'  { $$ = UnaryOpNode::UN_OP_NOT;   }
	;

cast_expression
	: unary_expression                       { $$ = $1;                   }
	| '(' type_specifier ')' cast_expression { $$ = new CastNode($2, $4); } //new type(TypePtr), expr to cast(NodePtr)
	;

new_expression
    : NEW type_specifier '(' argument_expression_list ')' { $$ = new NewNode($2, $4);      }
    | NEW type_specifier                                  { $$ = new NewNode($2);          }
    | NEW type_specifier '[' expression ']'               { $$ = new NewArrayNode($2, $4); }
    ;

multiplicative_expression
	: cast_expression                               { $$ = $1;                  }
	| multiplicative_expression '*' cast_expression { $$ = new MulNode($1, $3); }
	| multiplicative_expression '/' cast_expression { $$ = new DivNode($1, $3); }
	| multiplicative_expression '%' cast_expression { $$ = new ModNode($1, $3); }
	;

additive_expression
	: multiplicative_expression                         { $$ = $1; }
	| additive_expression '+' multiplicative_expression { $$ = new AddNode($1, $3); }
	| additive_expression '-' multiplicative_expression { $$ = new SubNode($1, $3); }
	;

shift_expression
	: additive_expression                           { $$ = $1; }
	| shift_expression LEFT_OP additive_expression  { $$ = new LshNode($1, $3); }
	| shift_expression RIGHT_OP additive_expression { $$ = new RshNode($1, $3); }
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '<' shift_expression   { $$ = new LessNode($1, $3); } //maybe add a TEST instuction to our assembly (and a FLAG register!)
	| relational_expression '>' shift_expression   { $$ = new GreaterNode($1, $3); }
	| relational_expression LE_OP shift_expression { $$ = new LENode($1, $3); }
	| relational_expression GE_OP shift_expression { $$ = new GENode($1, $3); }
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQ_OP relational_expression { $$ = new EqualNode($1, $3); }
	| equality_expression NE_OP relational_expression { $$ = new NEqualNode($1, $3); }
	;

and_expression
	: equality_expression { $$ = $1; }
	| and_expression '&' equality_expression { $$ = new BitAndNode($1, $3); }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression { $$ = new BitXorNode($1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression { $$ = new BitOrNode($1, $3); }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression { $$ = new AndNode($1, $3); }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression { $$ = new OrNode($1, $3); }
	;

conditional_expression
	: logical_or_expression { $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression { $$ = new ThreeNode($1, $3, $5); }
	;

assignment_expression //espressione che produce un r-value
	: conditional_expression { $$ = $1; }
	| logical_or_expression assignment_operator assignment_expression { $$ = new AssignementNode($1, $3, (AssignementNode::ASSIGN_TYPE)$2); }
	;

assignment_operator
	: '='           { $$ = AssignementNode::ASSIGN; }
	| MUL_ASSIGN    { $$ = AssignementNode::ASSIGN_MUL; }
	| DIV_ASSIGN    { $$ = AssignementNode::ASSIGN_DIV; }
	| MOD_ASSIGN    { $$ = AssignementNode::ASSIGN_MOD; }
	| ADD_ASSIGN    { $$ = AssignementNode::ASSIGN_ADD; }
	| SUB_ASSIGN    { $$ = AssignementNode::ASSIGN_SUB; }
	| LEFT_ASSIGN   { $$ = AssignementNode::ASSIGN_LEFT; }
	| RIGHT_ASSIGN  { $$ = AssignementNode::ASSIGN_RIGHT; }
	| AND_ASSIGN    { $$ = AssignementNode::ASSIGN_AND; }
	| XOR_ASSIGN    { $$ = AssignementNode::ASSIGN_XOR; }
	| OR_ASSIGN     { $$ = AssignementNode::ASSIGN_OR; }
	;

expression
	: assignment_expression                 { $$ = $1; }
	| expression ',' assignment_expression  { $$ = new ExprNode($1, $3); }
	;

constant_expression
	: conditional_expression
	;

%%
#include <stdio.h>

extern char yytext[];

void yyerror(char* s)
{
  ErrorManager::GetSingleton().logAndRaise(s);
}
