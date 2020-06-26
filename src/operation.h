#ifndef _OPERATION_H_
#define _OPERATION_H_

//isso aqui vai ajudar a gente a saber quando comeca um parenteses e quando não
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
    int reduced;
} operacoes;
//essa struct representa o nó da lista ligada da operacao
//dir e esq é pra poder navegar na lista ligada normal
//cima é pra caso haja parenteses, pra desviar o fluxo
//num é o conteúdo do nó, e op é a operacao
//ex: 1+1, o primeiro nó teria num = 1 e op = ADD
//o segundo teria num = 1 e op = EOE
//reduced é a flag pra vrer se o numero no nó atual foi resultado de uma operacao
//ou se ele já estava na conta

typedef struct pilhaOperacoes{
	operacoes * operacao;
	struct pilhaOperacoes *anterior;
}pilhaOperacoes;
//é uma pilha de todas as operacoes do programa, que é o que vai ser resovido pelo shift reduce


//operacoes na pilha com nome intuitivo do que fazem
pilhaOperacoes *criarPilha();

void adicionaPilha(pilhaOperacoes *pilha, operacoes *op);

operacoes *tiraDaPilha(pilhaOperacoes *pilha);

operacoes *criaLista(int valor, operadores op);

//Popula prox vai criar o poximo no da lista ligada
//pra isso, ele precisa receber o valor, o operador, a lista e uma flag
//essa flag diz pra ele se é o começo de um parenteses ou não
//ex: (1+2), no nó de 1, ele teria a flag = parentese, e o 2 teria flag = direita,
// como declarados no comeco do .h
operacoes *populaprox(int valor, operadores op, operacoes *cabeca, int flag);

//recebe a lista ligada e resolve a operacao
int resolveShiftReduce(operacoes *cabeca);

//funcoes de debug
void printaTudo(operacoes *cabeca);

char printaOperador(operadores op);

#endif
