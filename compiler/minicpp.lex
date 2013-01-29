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
"/*"                            { eatMultilineComment(); }
"//".*                          { SM.incrementLineNumber(); }
"#include"{SP}+\"[^\"]*\""\n"   { handleInclude(); }
"bool"                          { return(BOOL); }
"native"                        { return(NATIVE); }
"break"		                	{ return(BREAK); }
"case"		                	{ return(CASE); }
"char"		                	{ return(CHAR); }
"class"                         { return(CLASS); }
"const"		                	{ return(CONST); }
"continue"	                	{ return(CONTINUE); }
"default"	                	{ return(DEFAULT); }
"do"		                	{ return(DO); }
"else"		                	{ return(ELSE); }
"enum"		                	{ return(ENUM); }
"false"                         { return(FALSE); }
"float"		                	{ return(FLOAT); }
"for"                           { return(FOR); }
"if"                            { return(IF); }
"int"                           { return(INT); }
"long"		                	{ return(LONG); }
"new"                           { return(NEW); }
"private"{SP}*":"               { return(PRIVATE); }
"public"{SP}*":"                { return(PUBLIC); }
"return"	                	{ return(RETURN); }
"short"		                	{ return(SHORT); }
"signed"	                	{ return(SIGNED); }
"sizeof"		                { return(SIZEOF); }
"static"		                { return(STATIC); }
"string"                        { return(STRING); }
"struct"	                	{ return(STRUCT); }
"switch"	                	{ return(SWITCH); }
"this"                          { return(THIS); }
"true"                          { return(TRUE); }
"typedef"		                { return(TYPEDEF); }
"union"			                { return(UNION); }
"unsigned"                      { return(UNSIGNED); }
"virtual"                       { return(VIRTUAL); }
"void"			                { return(VOID); }
"while"			                { return(WHILE); }

0[xX]{H}+{IS}?         { 
                         int i;
                         //hex
                         sscanf(yytext,"%x",&i);
                         yylval.intval = i;                         
                         return(INT_CONSTANT);
                       }
0{D}+{IS}?             { 
                         int i;
                         //octal
                         sscanf(yytext,"%o",&i);
                         yylval.intval = i;
                         return(INT_CONSTANT);                          
                       }
{D}+{IS}?              { 
                         int i;
                         //decimal
                         sscanf(yytext,"%d",&i);
                         yylval.intval = i;
                         return(INT_CONSTANT);                          
                       }
                       
L?'(\\.|[^\\'])+'      {                          
                         yylval.name = yytext;
                         return(STRING_LITERAL); 
                       }

{D}+{E}{FS}?           { 
                         float f;
                         sscanf(yytext,"%f",&f);
                         yylval.realval = f;
                         
                         return(FLOAT_CONSTANT); 
                       }
{D}*"."{D}+({E})?{FS}? { 
                         float f;
                         sscanf(yytext,"%f",&f);
                         yylval.realval = f;
                         return(FLOAT_CONSTANT); 
                       }
{D}+"."{D}*({E})?{FS}? { 
                         float f;
                         sscanf(yytext,"%f",&f);
                         yylval.realval = f;
                         return(FLOAT_CONSTANT); 
                       }

L?\"(\\.|[^\\"])*\"	   {                          
                         yylval.name = yytext;
                         return(STRING_LITERAL); 
                       }
{L}({L}|{D})*          { 
                         return(SM.checkType(yylval.intval));
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
";"             { return(';'); }
("{"|"<%")		{ return('{'); }
("}"|"%>")		{ return('}'); }
","			    { return(','); }
":"             { return(':'); }
"="             { return('='); }
"("             { return('('); }
")"             { return(')'); }
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

"\n"            { SM.incrementLineNumber(); }
[ \t\v\n\f]		{ }
<<EOF>>         { if (!handleEof()) return 0; }
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
      ErrorManager::GetSingleton().logAndRaise(error.c_str());
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
