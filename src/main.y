%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "operation.h"

void yyerror(char *c);
int yylex(void);
%}
%token Num '+' '*' '/' '^' '(' ')' EndOF;
%%
S:
  C S {printf("final: %d\n", $1);}
  |
  ;
C:
  E {}
  | E C {}
  ;

E:
  Num {}
  | '(' {}
  | Num '+' {}
  | Num '*' {}
  | Num '^' {}
  | Num '/' {}
  | ')' {}
  | ')' EndOF {}
  | Num EndOF {}
  ;

%%
void yyerror(char *s) {
}

int main() {
  yyparse();


  return 0;

}


operacoes* cabeca;

void verifica(int num, operadores op){
  static int comeco = 1;
  static int parenteses = 0;

}