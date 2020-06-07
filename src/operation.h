#ifndef _OPERATION_H_
#define _OPERATION_H_
#define parentese 0
#define direita 1

typedef enum operadores{
    ADD,
    MULT,
    DIV,
    EXP,
    EOE
} operadores;

typedef struct operacoes{
    int num;
    operadores op;
    struct operacoes *dir;
    struct operacoes *esq;
    struct operacoes *cima;
} operacoes;

typedef struct pilhaOperacoes{
	operacoes * operacao;
	struct pilhaOperacoes *anterior;
}pilhaOperacoes;

pilhaOperacoes *criarPilha();

void adicionaPilha(pilhaOperacoes *pilha, operacoes *op);

operacoes *tiraDaPilha(pilhaOperacoes *pilha);

operacoes *criaLista(int valor, operadores op);

operacoes *populaprox(int valor, operadores op, operacoes *cabeca, int flag);

int resolveShiftReduce(operacoes *cabeca);

void printaTudo(operacoes *cabeca);

char printaOperador(operadores op);

#endif
