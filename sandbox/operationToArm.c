#include <stdlib.h>
#include <stdio.h>
#include <math.h>


typedef enum Op{
	ADD, // Sum
	MUL, // Multiplication
	DIV, // Division
	EXP, // Exponential 
	EOE  // End of expression 
} Operator;
typedef struct Operation {
	struct Operation * next;
	struct Operation * previous;
	struct Operation * solving;
	int value;
	Operator operator;
} Operation; 

Operation * createHead(int value, Operator op){
	Operation * head = malloc(sizeof(Operation));
	head->value = value;
	head->next=NULL;
	head->previous=NULL;
	head->solving=NULL;
	head->operator = op;
	return head;
}

Operation * addOperationSolving(Operation *head, int value, Operator op){
	Operation * operation = malloc(sizeof(Operation));
	operation->value = value;
	operation->next=NULL;
	operation->solving=NULL;
	operation->previous=head;
	head->solving=operation;
	operation->operator = op;
	return operation;
}   

Operation * addOperation(Operation *head, int value, Operator op){
	Operation * operation = malloc(sizeof(Operation));
	operation->value = value;
	operation->next=NULL;
	operation->solving=NULL;
	operation->previous=head;
	head->next=operation;
	operation->operator = op;
	return operation;
}   

void printOperator(Operator operator){
	switch (operator){
		case ADD:
			printf(" + ");
			break;
		case MUL:
			printf(" * ");
			break;
		case DIV:
			printf(" / ");
			break;
		case EXP:
			printf(" ^ ");
			break;
		case EOE:
			printf(";");
			break;
		default:
			printf("..");
			break;
	}
}

void showExpression(Operation * head){
	if (head == NULL) return;
	if (head->value != -1) // parentese
		printf("%d ", head->value);
	if (head->solving != NULL){
		printf("(");
		showExpression(head->solving);
		printf(")");
	}
	printOperator(head->operator);
	showExpression(head->next);
}




int solveExpression(Operation * head){
	Operation *expression = NULL;
	Operation *aux;
	Operation *p;
	for (p = head; p != NULL; p = p->next){
		if (p->solving != NULL){
			p->value = solveExpression(p->solving);
		}	
		if (p == head){
			expression = createHead(head->value, head->operator);
			aux = expression;
		}else{
			expression = addOperation(expression,p->value,p->operator);
		}
	}
	int init = 0;
	printf("expression [before exp]: ");
	showExpression(aux);
	printf("\n");

	for (p = expression; p != NULL; p = p->previous){
		if (p->previous != NULL && p->previous->operator == EXP) {
			p->previous->value = pow(p->previous->value, p->value);
			p->previous->operator = MUL;
			p->value = 1;
		}
	}
	printf("expression [after exp]: ");
	showExpression(aux);
	printf("\n");
	for (p = aux ; p != NULL; p = p->next){
		if (p->next != NULL && p->operator == MUL) {
			p->next->value = p->next->value * p->value;
			p->operator = ADD;
			p->value = 0;
		} else if (p->next != NULL && p->operator == DIV) {
			p->next->value = p->value/p->next->value;
			p->operator = ADD;
			p->value = 0;
		}
	}
	printf("expression [after mul]: ");
	showExpression(aux);
	printf("\n");
	int return_value = 0;
	for (p = expression; p != NULL; p = p->previous){
		if (p->previous != NULL && p->previous->operator == ADD) {
			p->previous->value = p->previous->value + p->value;
			return_value = p->previous->value;
			p->value = 0;
		}
	}
	printf("expression [after add]: ");
	showExpression(aux);
	printf("\n");
	

	
	return return_value;
}


int main(){
	Operation * o = createHead(-1,MUL);
	Operation *p, *q;
	q = addOperationSolving(o, 2, ADD);
	q = addOperation(q, 5, EOE);
	p = addOperation(o, 3, EXP);
	q = addOperation(p, -1, EOE);  
	q = addOperationSolving(q, 6, DIV);
      	q = addOperation(q,2, EOE);	
	showExpression(o);
	printf("\n");
	printf("resultado: %d\n",solveExpression(o));
	/*
	 * [HEAD] -> 10 + 5;
	 */
	
	return 0;
}
