%{


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <new>

#include "Types.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE

%}

%union 
{  
  char*      string; 
  TypePtr    type;
  int        intval;
  Function*  func;
  ClassType* ctype;
}

///I nostri vari token

%token <string> 

%token VIRTUAL PUBLIC PRIVATE PROTECTED 
%token CHAR SHORT INT LONG SHORT SIGNED UNSIGNED FLOAT CONST BOOL VOID STRING 
%token SIGNED_LONG SIGNED_SHORT SIGNED_INT UNSIGNED_INT UNSIGNED_LONG UNSIGNED_SHORT
%token WCHAR SIGNED_CHAR UNSIGNED_CHAR
%token STRUCT CLASS
%token DOTDOT NEW DELETE
%token EQ_OP ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN XOR_ASSIGN 
%token AND_ASSIGN OR_ASSIGN LSH_OP RSH_OP RIGHT_ASSIGN LEFT_ASSIGN NE_OP LE_OP
%token GE_OP AND_OP OR_OP INC_OP DEC_OP ARROW_STAR_OP ARROW_OP LPAR_RPAR LSQUARE_RSQUARE OPERATOR 
%token FASTCALL STDCALL THISCALL CDECL

%type <type> type_specifier parameter_decl_list type_decl
%type <intval> access_modifier call_convention
%type <func> operator func_identifier
%type <ctype> class_identifier

%token <name> IDENTIFIER

%start function_decl
%%

function_decl
  //constructor/destructor
  : access_modifier VIRTUAL call_convention class_identifier DOTDOT class_identifier '(' param_decl_list ')'
  | access_modifier VIRTUAL call_convention class_identifier DOTDOT '~' class_identifier '(' param_decl_list ')'
  | access_modifier type_decl call_convention class_identifier DOTDOT func_identifier '(' param_decl_list ')'
  //non member
  | type_decl call_convention func_identifier '(' param_decl_list ')'
  //non-mangled function
  | func_identifier
  ;
  
func_identifier
  : IDENTIFIER        { $$ = new Function($1); }
  | OPERATOR operator { $$ = $1; )
  ;
    
operator
  : NEW               { $$ = new Operator(NEW_OP); }         
  | DELETE            { $$ = new Operator(DELETE_OP); }
  | NEW '['']'        { $$ = new Operator(NEWA_OP); }
  | DELETE '['']'     { $$ = new Operator(DELETEA_OP); }
  | '+'               { $$ = new Operator(PLUS_OP); }
  | '-'               { $$ = new Operator(MINUS_OP); }
  | '*'               { $$ = new Operator(TIMES_OP); }
  | '/'               { $$ = new Operator(DIV_OP); }
  | '%'               { $$ = new Operator(MOD_OP); }
  | '^'               { $$ = new Operator(BIT_XOR_OP); }
  | '&'               { $$ = new Operator(BIT_AND_OP); }
  | '|'               { $$ = new Operator(BIT_OR_OP); }
  | '~'               { $$ = new Operator(BIT_NOT_OP); }
  | '!'               { $$ = new Operator(NOT_OP); }
  | '='               { $$ = new Operator(ASSIGN); }
  | EQ_OP             { $$ = new Operator(EQ_OP); }
  | '<'               { $$ = new Operator(L_OP); }
  | '>'               { $$ = new Operator(G_OP); }
  | ADD_ASSIGN        { $$ = new Operator(ADD_ASSIGN); }
  | SUB_ASSIGN        { $$ = new Operator(SUB_ASSIGN); }
  | MUL_ASSIGN        { $$ = new Operator(MUL_ASSIGN); }
  | DIV_ASSIGN        { $$ = new Operator(DIV_ASSIGN); }
  | MOD_ASSIGN        { $$ = new Operator(MOD_ASSIGN); }
  | XOR_ASSIGN        { $$ = new Operator(XOR_ASSIGN); }
  | AND_ASSIGN        { $$ = new Operator(AND_ASSIGN); }
  | OR_ASSIGN         { $$ = new Operator(OR_ASSIGN); }
  | LSH_OP            { $$ = new Operator(LSH_OP); }
  | RSH_OP            { $$ = new Operator(RSH_OP); }
  | RIGHT_ASSIGN      { $$ = new Operator(RIGHT_ASSIGN); }
  | LEFT_ASSIGN       { $$ = new Operator(LEFT_ASSIGN); }
  | NE_OP             { $$ = new Operator(NE_OP); }
  | LE_OP             { $$ = new Operator(LE_OP); }
  | GE_OP             { $$ = new Operator(GE_OP); }
  | AND_OP            { $$ = new Operator(AND_OP); }
  | OR_OP             { $$ = new Operator(OR_OP); }
  | INC_OP            { $$ = new Operator(INC_OP); }
  | DEC_OP            { $$ = new Operator(DEC_OP); }
  | ','               { $$ = new Operator(COMMA_OP); }
  | ARROW_STAR_OP     { $$ = new Operator(ARROW_STAR_OP); }
  | ARROW_OP          { $$ = new Operator(ARROW_OP); }
  | LPAR_RPAR         { $$ = new Operator(LPAR_RPAR); }
  | LSQUARE_RSQUARE   { $$ = new Operator(LSQUARE_RSQUARE ); }
  ;

call_convention
  : THISCALL          { $$ = CALL_THISCALL; }
  | STDCALL           { $$ = CALL_STDCALL; }
  | CDECL             { $$ = CALL_CDECL; }  
  | FASTCALL          { $$ = CALL_FASTCALL; }
  ;

type_decl
  : type_specifier    { $$ = $1; }
  | type_decl '&'     { $$ = TypeDef::createParam($1, true); }
  | type_decl CONST   { $$ = TypeDef::createConst($1); }
  | type_decl '*'     { $$ = TypeDef::createPointer($1); }
  ;
  
class_identifier
  : IDENTIFIER  
    {
      ClassTable::GetSingleton().get($1);
    }
  | IDENTIFIER '<' parameter_decl_list '>' 
    {
      ClassType& ct = ClassTable::GetSingleton().get($1);
      ct.addTemplateType($3);
    }
  | IDENTIFIER DOTDOT class_identifier     
    { 
      $$ = ClassTable::GetSingleton().addSubType($1, $3); 
    }
  ;
  
type_specifier
	: VOID              { $$ = TypeDef::getStdType(TYPE_VOID);   }
	| CHAR              { $$ = TypeDef::getStdType(TYPE_INT);    }
	| SHORT             { $$ = TypeDef::getStdType(TYPE_INT);    }
	| INT               { $$ = TypeDef::getStdType(TYPE_INT);    }
	| LONG              { $$ = TypeDef::getStdType(TYPE_INT);    }
	| FLOAT             { $$ = TypeDef::getStdType(TYPE_FLOAT);  }
	| SIGNED_INT        { $$ = TypeDef::getStdType(TYPE_INT);    }
	| UNSIGNED_INT      { $$ = TypeDef::getStdType(TYPE_INT);    }
  | SIGNED_LONG       { $$ = TypeDef::getStdType(TYPE_INT);    }
	| UNSIGNED_LONG     { $$ = TypeDef::getStdType(TYPE_INT);    }
  | SIGNED_SHORT      { $$ = TypeDef::getStdType(TYPE_INT);    }
	| UNSIGNED_SHORT    { $$ = TypeDef::getStdType(TYPE_INT);    }
  | SIGNED_CHAR       { $$ = TypeDef::getStdType(TYPE_CHAR);   }
	| UNSIGNED_CHAR     { $$ = TypeDef::getStdType(TYPE_CHAR);   }
  | STRING            { $$ = TypeDef::getStdType(TYPE_STRING); }
  | BOOL              { $$ = TypeDef::getStdType(TYPE_BOOL);   }
  | WCHAR             { $$ = TypeDef::getStdType(TYPE_WCHAR);  }
	| CLASS class_identifier  
    { 
      /*
      TypePtr classType = new TypeDef(); 
      classType->type_id = TYPE_CLASS;
      classType->name = $1;
      $$ = classType;
      */
      $$ = $1;
    }
	;


access_modifier
  : PUBLIC    { $$ = ACCESS_PUBLIC;    }
  | PRIVATE   { $$ = ACCESS_PRIVATE;   }
  | PROTECTED { $$ = ACCESS_PROTECTED; }
  ;  

parameter_decl_list
  : type_decl                         { $$ = $1; } 
  | type_decl ',' parameter_decl_list { $$ = TypeDef::createProd($1, $3); }
  ;
  

  
%%
#include <stdio.h>

extern char yytext[];

void yyerror(char* s)
{
  fprintf(stderr, "Error: %s\n", s);
}
