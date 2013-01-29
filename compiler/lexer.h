
#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <string>

int yylex(void);
void yyerror(char *s);

class lexer
{
public:
  static void openFile(const std::string& filename);
};

void count();
int check_type();
void incrementLineNumber();

int yyparse(void);

extern int yydebug;

#endif //LEXER_H_INCLUDED

