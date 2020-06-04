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
  S E {printf("final: %d\n", $2);}
  |
  ;

%%
void yyerror(char *s) {
}

int main() {
  yyparse();
    return 0;

}
