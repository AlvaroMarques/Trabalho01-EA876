%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "operation.h"
void yyerror(char *c);
int yylex(void);
%}
%token Num '+' '*' '/' '^' '(' ')';
%%
S:
  E S {printf("final: %d\n", $2);}
  |
  ;

E:
  E {}
  | Num {}
  | '(' {}
  | Num '+' {}
  | Num '*' {}

%%
void yyerror(char *s) {
}

int main() {
  yyparse();


  return 0;

}
