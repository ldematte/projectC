D [0-9]
L [a-zA-Z_]
H [a-fA-F0-9]
E [Ee][+-]?{D}+
FS (f|F|l|L)
IS (u|U|l|L)*
SP [ \t]

%{
#include <stdio.h>

#include <string>
#include <stack>

#include "type.h"
#include "SyntaxTree.h"
#include "SourceManager.h"
#include "ErrorManager.h"

#include "minicpp.cpp.h"

#define SM SourceManager::GetSingleton() 

bool handleEof();
void handleInclude();
void eatMultilineComment();

#include "lexer.h"
#define   SKIP_YYWRAP		    1
#define   YY_NEVER_INTERACTIVE	1

struct includeinfo_t
{
  YY_BUFFER_STATE buf;
  int line;
  std::string filename;
};

std::stack<includeinfo_t> Files;

%}

%option yylineno

%%
"bool"                      { return (BOOL); }
"char"		                	{ return (CHAR); }
"wchar"		                	{ return (WCHAR); }
"unsigned char"             { return (UNSIGNED_CHAR); }
"signed short"              { return (SIGNED_CHAR); } 
"class"                     { return (CLASS); }
"const"		                	{ return (CONST); }
"float"		                	{ return (FLOAT); }
"double"                    { return (DOUBLE); }
"int"                       { return (INT); }
"unsigned int"              { return (UNSIGNED_INT); }
"signed int"                { return (SIGNED_INT); } 
"long"		                	{ return (LONG); }
"unsigned long"             { return (UNSIGNED_LONG); }
"signed long"               { return (SIGNED_LONG); } 
"new"                       { return (NEW); }
"private"{SP}*":"           { return (PRIVATE); }
"public"{SP}*":"            { return (PUBLIC); }
"return"	                	{ return (RETURN); }
"short"		                	{ return (SHORT); }
"unsigned short"            { return (UNSIGNED_SHORT); }
"signed short"              { return (SIGNED_SHORT); } 
"signed"	                	{ return (SIGNED_INT); }
"string"                    { return (STRING); }
"struct"	                	{ return (STRUCT); }
"unsigned"                  { return (UNSIGNED_INT); }
"virtual"                   { return (VIRTUAL); }
"void"			                { return (VOID); }
"operator"                  { return (OPERATOR); }
"__thiscall"                { return (THISCALL); }
"__stdcall"                 { return (STDCALL); }
"__cdecl"                   { return (CDECL); }
"::"                        { return (DOTDOT); }

{L}({L}|{D})*          { 
                         return(IDENTIFIER);
                       }


">>="			{ return(RIGHT_ASSIGN); }
"<<="			{ return(LEFT_ASSIGN); }
"+="			{ return(ADD_ASSIGN); }
"-="			{ return(SUB_ASSIGN); }
"*="			{ return(MUL_ASSIGN); }
"/="			{ return(DIV_ASSIGN); }
"%="			{ return(MOD_ASSIGN); }
"&="			{ return(AND_ASSIGN); }
"^="			{ return(XOR_ASSIGN); }
"|="			{ return(OR_ASSIGN); }
">>"			{ return(RIGHT_OP); }
"<<"			{ return(LEFT_OP); }
"++"			{ return(INC_OP); }
"--"			{ return(DEC_OP); }
"&&"			{ return(AND_OP); }
"||"			{ return(OR_OP); }
"<="			{ return(LE_OP); }
">="			{ return(GE_OP); }
"=="			{ return(EQ_OP); }
"!="			{ return(NE_OP); }
"->*"     { return (ARROW_STAR_OP); }
"->"      { return (ARROW_OP); }
";"             { return(';'); }
("{"|"<%")		{ return('{'); }
("}"|"%>")		{ return('}'); }
","			    { return(','); }
":"             { return(':'); }
"="             { return('='); }
"("             { return('('); }
")"             { return(')'); }
"()"            { return(LPAR_RPAR); }
"[]"            { return(LSQUARE_RSQUARE); }
("["|"<:")		{ return('['); }
("]"|":>")		{ return(']'); }
"."             { return('.'); }
"&"             { return('&'); }
"!"             { return('!'); }
"~"             { return('~'); }
"-"             { return('-'); }
"+"             { return('+'); }
"*"             { return('*'); }
"/"             { return('/'); }
"%"             { return('%'); }
"<"             { return('<'); }
">"             { return('>'); }
"^"             { return('^'); }
"|"             { return('|'); }
"?"             { return('?'); }
[ \t\v\n\f]		{ }
.               { /* ignore bad characters */ }

%%

void eatMultilineComment()
{
   register int c; 
   for ( ; ; ) 
   { 
      while(1)
	  {
	     c = yyinput(); if (c == '\n') SM.incrementLineNumber();
		 if (c == '*' || c == EOF) break;
	  }      
      if ( c == '*' ) 
      { 
	     while ( (c = yyinput()) == '*' ); 
		 if (c == '\n') SM.incrementLineNumber();
		 if ( c == '/' ) 
            break; /* found the end */ 
      } 
	  if ( c == EOF ) 
      { 
	     printf( "Trovato EOF in commento!\n" ); 
		 exit(1);
         break; 
      } 
   } 
}

bool handleEof()
{
   if (Files.empty()) return false;
   
   yy_delete_buffer(YY_CURRENT_BUFFER);
   yy_switch_to_buffer(Files.top().buf);
   SM.setCurrentLineNumber(Files.top().line + 1);
   SM.setCurrentFile(Files.top().filename);

   Files.pop();
      
   return true;   
}

void handleInclude()
{
   includeinfo_t ii;
   ii.buf = YY_CURRENT_BUFFER;
   ii.line = SM.getCurrentLineNumber();
   ii.filename = SM.getCurrentFile();
   Files.push(ii);   

   std::string filename = SM.getIncludeFile(yytext);
   yyin = fopen(filename.c_str(), "r");
   if (!yyin)
   {
      std::string error = "Impossibile aprire il file include: " + filename;
      ErrorManager::GetSingleton().logAndRaise(error);
   }
   
   SM.setCurrentFile(filename);
   yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
   BEGIN(INITIAL);
}

void lexer::openFile(const std::string& filename)
{
  yyin = fopen(filename.c_str(), "r");
  SM.setCurrentFile(filename);
};

//hacks
int yywrap()
{
	return(1);
}

int isatty(int i)
{
  return 0;
}
