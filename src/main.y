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
  C {resolveShiftReduce(operacaoInicial);}
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
  printf("\n");
  int c;
  //essa file é a funcao exponencial ema arm feita em outro arquivo
  FILE *file;
  file = fopen("./src/exponential.s", "r");
  if (file) {
      while ((c = getc(file)) != EOF)
  	putchar(c);
      fclose(file);
  }
}

void verifica(int num, operadores op){

  operacoes *operacao;
  static int comeco = 1;
  static int paren = parentese;

  //aqui, a gente precisa de um marco pro inicio da pilha, pra saber quando criar a lista
  if (comeco == 1){
    operacaoInicial = criaLista(num, op);
    adicionaPilha(cabeca, operacaoInicial);
    //o num ser = -1 quer dizer que a gente vai iniciar um parentese
    if (num == -1){
      paren = parentese;
    }
    else{
      paren = direita;
    }
    comeco = 0;
    return;
  }
  //Leia o adendo 1 do readme pra saber como funciona melhor essa lógica
  if (paren == parentese){
    operacao = populaprox(num, op, cabeca->anterior->operacao, paren);
  }
  else{
    operacao = populaprox(num, op, tiraDaPilha(cabeca), paren);
  }
  //aqui adiciona na pilha quando não é o comeco
  adicionaPilha(cabeca, operacao);
  if (num == -1){
      paren = parentese;
    }
    else{
      paren = direita;
    }
  return;
}
