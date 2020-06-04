#include <stdio.h>
#include <stdlib.h>
#include "operation.h"

operacoes *criaLista(int valor, operadores op){
    operacoes *vetor = malloc(sizeof(operacoes));
    vetor->num = valor;
    vetor->op = op;
    vetor->cima = NULL;
    vetor->dir = NULL;
    vetor->esq = NULL;
    return vetor;
}

operacoes *populaprox(int valor, operadores op, operacoes *cabeca, int flag){
    if ((flag) == parentese ){
        cabeca->cima = criaLista(valor, op);
        return cabeca->cima;
    }
    else{
        cabeca->dir = criaLista(valor, op);
        cabeca->dir->esq = cabeca;
        return cabeca->dir;
    }
}

int resolveShiftReduce(operacoes *cabeca){
    if (cabeca->num == -1){
        cabeca->num = resolveShiftReduce(cabeca->cima);
        resolveShiftReduce(cabeca);
    }
    else{
        if (cabeca->op == ADD){
            if (cabeca->dir->op == ADD){
                cabeca->dir->num = cabeca->dir->num + cabeca->num;
            }
            if (cabeca->dir->op == MULT || cabeca->dir->op == DIV)
            {
                resolveShiftReduce(cabeca->dir);
                resolveShiftReduce(cabeca);
            }
            if (cabeca->dir->op == EXP){
                
            }
        }

        else if (cabeca->op == MULT){
            if (cabeca->dir->op == ADD){
                cabeca->dir->num = cabeca->num * cabeca->dir->num;
                cabeca->num = 0;
                cabeca->op = ADD;
            }
            if (cabeca->dir->op == MULT || cabeca->dir->op == MULT){
                cabeca->dir->num = cabeca->num * cabeca->dir->num;
                cabeca->num = 0;
                cabeca->op = ADD;
            }
            if (cabeca->dir->op == EXP){

            }
        }
        else if (cabeca->op == DIV){
            if (cabeca->dir->op == ADD){
                cabeca->dir->num = cabeca->num / cabeca->dir->num;
                cabeca->num = 0;
                cabeca->op = ADD;
            }
            if (cabeca->dir->op == MULT || cabeca->dir->op == MULT){
                cabeca->dir->num = cabeca->num / cabeca->dir->num;
                cabeca->num = 0;
                cabeca->op = ADD;
            }
            if (cabeca->dir->op == EXP)
            {
            }
        }

        else if (cabeca->op == EXP){
            if (cabeca->dir->op == ADD){
            }
            if (cabeca->dir->op == MULT){
            }
            if (cabeca->dir->op == EXP){
            }
        }
    }
}


char printaOperador(operadores op){
    switch (op)
    {
    case ADD:
        return '+';
        break;
    case MULT:
        return '*';
        break;
    case DIV:
        return '/';
        break;
    case EXP:
        return '^';
        break;
    default:
        return ';';
        break;
    }
}

void printaTudo(operacoes *cabeca){
    if (cabeca == NULL){
        return;
    }
    else if (cabeca->num == -1){
        printf("(");
        printaTudo(cabeca->cima);
        printf(")");
    }
    if (cabeca->num == -1){
        printf("%c", printaOperador(cabeca->op));
        printaTudo(cabeca->dir);
    }
    else{
        printf("%d %c", cabeca->num, printaOperador(cabeca->op));
        printaTudo(cabeca->dir);
    }
}

int main(){
    operacoes* conta = criaLista(-1, MULT);
    operacoes* auxConta = populaprox(2, ADD, conta, parentese);
    auxConta = populaprox(5, EOE, auxConta, direita);
    auxConta = populaprox(-1, EOE, conta, direita);
    auxConta = populaprox(3, EXP, auxConta, parentese);
    auxConta = populaprox(2, EOE, auxConta, direita);
    printaTudo(conta);
    printf("\n");

    return 0;
}