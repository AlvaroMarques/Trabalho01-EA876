%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "operation.h"

pilhaOperacoes *cabeca;
operacoes *operacaoInicial;
void verifica(int num, operadores op);
void yyerror(char *c);
int yylex(void);
%}
%token Num '+' '*' '/' '^' '(' ')' EndOF;
%%
S:
  C S {printaTudo(operacaoInicial);printf("\n");}
  |
  ;
C:
  E {}
  | E C {}
  ;

E:
  Num {verifica($1, EOE);}
  | '(' {verifica(-1, EOE);}
  | Num '+' {verifica($1, ADD);}
  | Num '*' {verifica($1, MULT);}
  | Num '/' {verifica($1, DIV);}
  | Num '^' {verifica($1, EXP);}
  | ')' '+' {tiraDaPilha(cabeca);
              cabeca->anterior->operacao->op = ADD;}
  | ')' '*' {tiraDaPilha(cabeca);
              cabeca->anterior->operacao->op = MULT;}
  | ')' '/' {tiraDaPilha(cabeca);
              cabeca->anterior->operacao->op = DIV;}
  | ')' '^' {tiraDaPilha(cabeca);
              cabeca->anterior->operacao->op = EXP;}
  | ')' EndOF {}
  | Num EndOF {verifica($1, ADD);}
  ;

%%
void yyerror(char *s) {
}

int main() {
  cabeca = criarPilha(NULL);
  yyparse();
  
  // pilhaOperacoes *temp;

  // for (temp = cabeca; temp->anterior->anterior != NULL; temp = cabeca->anterior){

  // }

  // printaTudo(temp->operacao);

  // return 0;

}

void verifica(int num, operadores op){

  operacoes *operacao;
  static int comeco = 1;
  static int paren = parentese;

  if (comeco == 1){
    operacaoInicial = criaLista(num, op);
    adicionaPilha(cabeca, operacaoInicial);
    if (num == -1){
      paren = parentese;
    }
    else{
      paren = direita;
    }
    comeco = 0;
    return;
  } 
  operacao = populaprox(num, op, tiraDaPilha(cabeca), paren);
  if (num == -1){
      paren = parentese;
    }
    else{
      paren = direita;
    }
  return;
}