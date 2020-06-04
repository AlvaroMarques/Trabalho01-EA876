#include <stdio.h>
#include <stdlib.h>
#include "operation.h"

typedef enum operadores{
    ADD,
    MULT,
    DIV,
    EXP,
    EOE
}operadores;

typedef struct operacoes{
    int num;
    operadores op;
    struct operacoes *dir;
    struct operacoes *esq;
    struct operacoes *cima;
}operacoes;
