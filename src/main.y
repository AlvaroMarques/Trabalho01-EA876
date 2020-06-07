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
  C {printf("%d",resolveShiftReduce(operacaoInicial));}
  |
  ;
C:
  E {}
  | C E {}
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
  | ')' EndOF {tiraDaPilha(cabeca);}
  | ')' {tiraDaPilha(cabeca);}
  | Num EndOF {verifica($1, EOE);}
  ;

%%
void yyerror() {
}

int main() {
  cabeca = criarPilha(NULL);
  yyparse();
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
  if (paren == parentese){
    operacao = populaprox(num, op, cabeca->anterior->operacao, paren);
  }
  else{
    operacao = populaprox(num, op, tiraDaPilha(cabeca), paren);
  }
  
  adicionaPilha(cabeca, operacao);
  if (num == -1){
      paren = parentese;
    }
    else{
      paren = direita;
    }
  return;
}
