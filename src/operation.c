#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "operation.h"

//recebe o valor e o operador iniciais, retorna o comeca da lista criada
operacoes *criaLista(int valor, operadores op){
	operacoes *vetor = malloc(sizeof(operacoes));
	vetor->num = valor;
	vetor->op = op;
	vetor->cima = NULL;
	vetor->dir = NULL;
	vetor->esq = NULL;
	vetor->reduced = 0;
	return vetor;
}

//recebe o valor, o operador, o vetor de operacoes e a flag do parentese
//retorna o nó criado
//a ideia é que se flag = parentese, a inves de continuarmos a escrever a operacao em cabeca->dir,
//escrevemos em cabeca->cima, como se criassemos uma branch nova pra resolver o conteúdo do parenteses
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
//aqui começa o Shift-Reduce
//A ideia aqui é olhar sempre pra operacao que eu estou e pra proxima(dir)
//E tomar uma decisão baseada nisso
int resolveShiftReduce(operacoes *cabeca){
	static operacoes *c = NULL;
	static int redux = 0;
    //essa variavel redux = 1 serve pra sabermos quando há um inicio de parentese
    //ja que o codigo em ARM é um pouco diferente caso estejamos no começo
	if (c == NULL || redux == 1) {
		redux = 0;
		if ((cabeca->op != EXP && cabeca->num != -1))
			printf("MOV A, %d\nPUSH A\n", cabeca->num);
		c = cabeca;
	}
    //cabeça->num ser -1 quer dizer que é o comeco de um parentese
	if (cabeca->num == -1){
		redux = 1;
		cabeca->num = resolveShiftReduce(cabeca->cima);
		cabeca->reduced = 1;
		return resolveShiftReduce(cabeca);
	}
	else{
        //isso aqui quer dizer que a proxima operacao terá um parentese, e prepara o codigo pra isso
		if (cabeca->dir != NULL && cabeca->dir->num == -1){
			redux = 1;
			cabeca->dir->num = resolveShiftReduce(cabeca->dir->cima);
			cabeca->dir->reduced = 1;
		}
        //A partir daqui, explicaremos no adendo 2 do ReadMe
        //ADD
		if (cabeca->op == ADD){
			if (cabeca->dir->op == ADD){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nADD A, B\nPUSH A\n");
				}else{
					printf("MOV B, %d\nPOP A\nADD A, B\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num + cabeca->dir->num;
				cabeca->reduced = cabeca->dir->reduced;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == MULT || cabeca->dir->op == DIV){
				printf("MOV A, %d\nPUSH A\n", cabeca->dir->num);
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
				cabeca->dir->reduced = 1;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EXP){
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
				cabeca->dir->reduced = 1;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EOE){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nADD A, B\nPUSH A\n");
				}else{
					printf("MOV B, %d\nPOP A\nADD A, B\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num + cabeca->dir->num;
				operacoes *tmp = cabeca->dir;
				cabeca->dir = tmp->dir;
				cabeca->op = tmp->op;
				return cabeca->num;
			}
		}
        //MULT
		else if (cabeca->op == MULT){
			if (cabeca->dir->op == ADD){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nMUL B\nPUSH A\n");
				}else{
					printf("POP A\nMUL %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num * cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == MULT || cabeca->dir->op == DIV){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nMUL B\nPUSH A\n");
				}else{
					printf("POP A\nMUL %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num * cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EXP){
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
				cabeca->dir->reduced = 1;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EOE){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nMUL B\nPUSH A\n");
				}else{
					printf("POP A\nMUL %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num * cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return cabeca->num;
			}
		}
        //DIV
		else if (cabeca->op == DIV){
			if (cabeca->dir->op == ADD){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nDIV B\nPUSH A\n");
				}else{
					printf("POP A\nDIV %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num / cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == MULT || cabeca->dir->op == DIV){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nDIV B\nPUSH A\n");
				}else{
					printf("POP A\nDIV %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num / cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EXP){
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
				cabeca->dir->reduced = 1;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EOE){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nDIV B\nPUSH A\n");
				}else{
					printf("POP A\nDIV %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num / cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return cabeca->num;
			}
		}
        //EXP
		else if (cabeca->op == EXP){
			if (cabeca->dir->op == ADD){
				if (cabeca->esq != NULL && cabeca->esq->op == EXP){
					if (cabeca->dir->reduced){
						printf("POP B\nPOP A\nCALL exp\nPUSH A\n");
					}else{
						printf("MOV A, %d\nMOV B, %d\nCALL exp\nPUSH A\n", cabeca->num, cabeca->dir->num);
					}
					cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
					operacoes *tmp = cabeca->dir;
					cabeca->dir = tmp->dir;
					cabeca->op = tmp->op;
					cabeca->reduced = 1;
					return resolveShiftReduce(cabeca->esq);
				}
				else{
					if (cabeca->dir->reduced){
						printf("POP B\nMOV A, %d\nCALL exp\nPUSH A\n",cabeca->num);
					}else{
						printf("MOV A, %d\nMOV B, %d\nCALL exp\nPUSH A\n", cabeca->num, cabeca->dir->num);
					}
					cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
					operacoes *tmp = cabeca->dir;
					cabeca->dir = tmp->dir;
					cabeca->op = tmp->op;
					cabeca->reduced = 1;
					return resolveShiftReduce(cabeca);
				}
			}
			if (cabeca->dir->op == MULT || cabeca->dir->op == DIV){
				if (cabeca->esq != NULL && cabeca->esq->op == EXP){
					if (cabeca->dir->reduced){
						printf("POP B\nPOP A\nCALL exp\nPUSH A\n");
					}else{
						printf("MOV A, %d\nMOV B, %d\nCALL exp\nPUSH A\n", cabeca->num, cabeca->dir->num);
					}
					cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
					operacoes *tmp = cabeca->dir;
					cabeca->dir = tmp->dir;
					cabeca->op = tmp->op;
					cabeca->reduced = 1;
					return resolveShiftReduce(cabeca->esq);
				}
				else{
					if (cabeca->dir->reduced){
						printf("POP B\nPOP A\nCALL exp\nPUSH A\n");
					}else{
						printf("MOV A, %d\nMOV B, %d\nCALL exp\nPUSH A\n", cabeca->num, cabeca->dir->num);
					}
					cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
					operacoes *tmp = cabeca->dir;
					cabeca->dir = tmp->dir;
					cabeca->op = tmp->op;
					cabeca->reduced = 1;
					return resolveShiftReduce(cabeca);
				}
			}
			if (cabeca->dir->op == EXP){
				printf("MOV A, %d\nPUSH A\n", cabeca->num);
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EOE){
				if (cabeca->esq != NULL && cabeca->esq->op == EXP){
					if (cabeca->dir->reduced == 1){
						printf("POP B\nPOP A\nCALL exp\nPUSH A\n");
					} else {
						printf("MOV A, %d\nMOV B, %d\nCALL exp\nPUSH A\n",cabeca->num,cabeca->dir->num);
					}
				}else{
					if (cabeca->dir->reduced == 1){
						printf("POP B\nPOP A\nCALL exp\nPUSH A\n");
					} else {
						printf("MOV A, %d\nMOV B, %d\nCALL exp\nPUSH A\n",cabeca->num,cabeca->dir->num);
					}
				}
				cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
				operacoes *tmp = cabeca->dir;
				cabeca->dir = tmp->dir;
				cabeca->op = tmp->op;
				cabeca->reduced = 1;
				return cabeca->num;
			}
		}
		else if (cabeca->op == EOE){
			return cabeca->num;
		}
	}
    //Esse -2 aqui nunca vai chegar, mas era uma forma da gente testar quando dava erro
	return -2;
}

pilhaOperacoes *criarPilha(operacoes *o){
	pilhaOperacoes *p = malloc(sizeof(pilhaOperacoes));
	p->anterior = NULL;
	p->operacao = o;
	return p;
}

void adicionaPilha(pilhaOperacoes *pilha, operacoes *operacao){
	pilhaOperacoes *n = criarPilha(operacao);
	n->anterior = pilha->anterior;
	pilha->anterior = n;
}

operacoes *tiraDaPilha(pilhaOperacoes *pilha){
	operacoes *tmp = pilha->anterior->operacao;
	pilhaOperacoes *aux = pilha->anterior;
	pilha->anterior = aux->anterior;
	return tmp;
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
		return ' ';
		break;
	}
}
//essa funcao recebe a lista ligada de todas as operacoes e printa recursivamente
//é uma funcao de debug
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

