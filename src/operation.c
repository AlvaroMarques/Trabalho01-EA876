#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "operation.h"

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
	static operacoes *c = NULL;
	if (c == NULL) {
		printf("MOV A, %d\nPUSH A\n", cabeca->num);
		c = cabeca;
	}
	//printaTudo(c);
	//printf("\n");
	if (cabeca->num == -1){
		cabeca->num = resolveShiftReduce(cabeca->cima);
		return resolveShiftReduce(cabeca);
	}
	else{
		if (cabeca->dir != NULL && cabeca->dir->num == -1){
			cabeca->dir->num = resolveShiftReduce(cabeca->dir->cima);
		}
		if (cabeca->op == ADD){
			if (cabeca->dir->op == ADD){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nADD A, B\nPUSH A\n");
				}else{
					printf("MOV B, %d\nPOP A\nADD A, B\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->dir->num = cabeca->dir->num + cabeca->num;
				cabeca->num = 0;
				return resolveShiftReduce(cabeca->dir);
			}
			if (cabeca->dir->op == MULT || cabeca->dir->op == DIV){
				printf("MOV A, %d\nPUSH A\n", cabeca->dir->num);
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EXP){
				printf("MOV A, %d\nPUSH A\n", cabeca->dir->num);
				cabeca->dir->reduced = 1;
				resolveShiftReduce(cabeca->dir);
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
		else if (cabeca->op == DIV){
			if (cabeca->dir->op == ADD){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nMUL B\nPUSH A\n");
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
					printf("POP B\nPOP A\nMUL B\nPUSH A\n");
				}else{
					printf("POP A\nDIV %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num / cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EXP){
				resolveShiftReduce(cabeca->dir);
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EOE){
				if (cabeca->dir->reduced){
					printf("POP B\nPOP A\nMUL B\nPUSH A\n");
				}else{
					printf("POP A\nDIV %d\nPUSH A\n", cabeca->dir->num);
				}
				cabeca->num = cabeca->num / cabeca->dir->num;
				cabeca->op = cabeca->dir->op;
				cabeca->dir = cabeca->dir->dir;
				return cabeca->num;
			}
		}

		else if (cabeca->op == EXP){
			if (cabeca->dir->op == ADD){
				printf("%d ^ %d = %d\n", cabeca->num, cabeca->dir->num, (int) pow(cabeca->num,cabeca->dir->num));
				cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
				operacoes *tmp = cabeca->dir;
				cabeca->dir = tmp->dir;
				cabeca->op = tmp->op;
				if (cabeca->esq != NULL && cabeca->esq->op == EXP){
					return resolveShiftReduce(cabeca->esq);
				}
				else{
					return resolveShiftReduce(cabeca);
				}
				//return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == MULT || cabeca->dir->op == DIV){
				printf("%d ^ %d = %d\n", cabeca->num, cabeca->dir->num, (int) pow(cabeca->num,cabeca->dir->num));
				cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
				operacoes *tmp = cabeca->dir;
				cabeca->dir = tmp->dir;
				cabeca->op = tmp->op;
				if (cabeca->esq != NULL && cabeca->esq->op == EXP){
					return resolveShiftReduce(cabeca->esq);
				}
				else{
					return resolveShiftReduce(cabeca);
				}
				//return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EXP){
				resolveShiftReduce(cabeca->dir);
				return resolveShiftReduce(cabeca);
			}
			if (cabeca->dir->op == EOE){
				printf("%d ^ %d = %d\n", cabeca->num, cabeca->dir->num, (int) pow(cabeca->num,cabeca->dir->num));
				cabeca->num = (int) pow(cabeca->num, cabeca->dir->num);
				operacoes *tmp = cabeca->dir;
				cabeca->op = tmp->op;
				return cabeca->num;
			}
		}
		else if (cabeca->op == EOE){
			cabeca->reduced = 0;
			return cabeca->num;
		}
	}
	return -5;
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

// int main(){
//	 // operacoes* conta = criaLista(1, ADD);
//	 // operacoes* auxConta = populaprox(1, ADD, conta, direita);
//	 // auxConta = populaprox(1, EOE, auxConta, direita);

//	 // operacoes* conta = criaLista(-1, MULT);
//	 // operacoes* auxConta = populaprox(2, ADD, conta, parentese);
//	 // auxConta = populaprox(5, EOE, auxConta, direita);
//	 // auxConta = populaprox(-1, EOE, conta, direita);
//	 // auxConta = populaprox(3, EXP, auxConta, parentese);
//	 // auxConta = populaprox(2, EOE, auxConta, direita);

//	 // operacoes* conta = criaLista(-1, ADD);
//	 // operacoes* auxConta = populaprox(1, EOE, conta, parentese);
//	 // auxConta = populaprox(-1, EOE, conta, direita);
//	 // auxConta = populaprox(1, EOE, auxConta, parentese);

//	 operacoes* conta = criaLista(-1, EOE);
//	 operacoes* auxConta = populaprox(-1, EOE, conta, parentese);
//	 auxConta = populaprox(-1, EOE, auxConta, parentese);
//	 auxConta = populaprox(-1, EOE, auxConta, parentese);
//	 auxConta = populaprox(-1, EOE, auxConta, parentese);
//	 auxConta = populaprox(5, ADD, auxConta, parentese);
//	  auxConta = populaprox(5, EOE, auxConta, direita);

//	 printaTudo(conta);
//	 printf("\n");
//	 printf("%d\n", resolveShiftReduce(conta));

//	 return 0;
// }
