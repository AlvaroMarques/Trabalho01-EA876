%{
  #include <stdio.h>
  #include <stdlib.h>
void yyerror(char *c);
int yylex(void);
%}
%token Num '+' '*' '/' '^' '(' ')';
%%
S:
  P S {printf("Valido");};
  |
  ;

P:
  E {};
  | '(' E ')'{};

E:
  Num {};
  | P OP P {};

OP:
  '^' {};
  | '*' {};
  | '/' {};
  | '+' {};

%%
void yyerror(char *s) {
  printf("invalido");
}

int main() {
  yyparse();
    return 0;

}
