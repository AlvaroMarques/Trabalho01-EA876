%{
  #include <stdio.h>
  #include <stdlib.h>
void yyerror(char *c);
int yylex(void);
%}
%token Num '+' '*' '/' '^' '(' ')';
%%
S:
  S SOMAP {printf("final: %d\n", $2);}
  |
  ;

SOMAP:
  SOMA {$$ = $1;}
  | '(' SOMA ')' {$$ = $2;}
  | '(' SOMAP ')' {$$ = $2;}
  ;

SOMA:
  | Num {$$ = $1;}
  | Num '+' Num {$$ = $1 + $3;}
  | MULTP '+' MULTP {$$ = $1 + $3;}
  ;

MULTP:
  MULT {$$ = $1;}
  | '(' MULT ')' {$$ = $2;}
  | '(' MULTP ')' {$$ = $2;}
  ;

MULT:
  Num {$$ = $1;}
  | Num '*' Num {$$ = $1 * $3;}
  | EXPP '*' EXPP {$$ = $1*$3;}
  | EXPP '/' EXPP {$$ = $1/$3;}
  | Num '/' Num {$$ = $1 / $3;}
  ;

EXPP:
  EXP {$$ = $1;}
  | '(' EXP ')' {$$ = $2;}
  | '(' EXPP ')' {$$ = $2;}
  ;

EXP:
  Num {$$ = $1;}
  | PAREN '^' PAREN {$$ = $1^$3;}
  | Num '^' Num {$$ = $1 * $3;}
  ;

PAREN:
  Num {$$ = $1;}
  | SOMAP {$$ = $1;}
  ;

%%
<<<<<<< Updated upstream
void yyerror() {
  printf("invalido");
=======
void yyerror(char *s) {
>>>>>>> Stashed changes
}

int main() {
  yyparse();
    return 0;

}
