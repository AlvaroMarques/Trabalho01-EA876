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

Operation * createHead(Operator op){
	Operation * head = malloc(sizeof(Operation));
	head->value = -1;
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

typedef struct NumberStack {
	struct NumberStack * previous;
	int value;
} NumberStack;

NumberStack * createNumberStack(){
	NumberStack * head = malloc(sizeof(NumberStack));
	head->value = -1;
	head->previous = NULL;
	return head;
}

void addToNumberStack(NumberStack * head, int value){
	NumberStack * new = malloc(sizeof(NumberStack));	
	new->value  = value;
	new->previous = head->previous;
	head->previous = new;
}

int detachValueNumberStack (NumberStack * head){
	NumberStack * freedom = head->previous;
	int tmp = freedom->value;
	head->previous = freedom->previous;
	free(freedom);
	return tmp;
}


typedef struct OperatorStack {
	struct OperatorStack * previous;
	Operator value;
} OperatorStack;
OperatorStack * createOperatorStack(){
	OperatorStack * head = malloc(sizeof(OperatorStack));
	head->value = EOE;
	head->previous = NULL;
	return head;
}

void addToOperatorStack(OperatorStack * head, int value){
	OperatorStack * new = malloc(sizeof(OperatorStack));	
	new->value  = value;
	new->previous = head->previous;
	head->previous = new;
}

int detachValueOperatorStack (OperatorStack * head){
	OperatorStack * freedom = head->previous;
	Operator tmp = freedom->value;
	head->previous = freedom->previous;
	free(freedom);
	return tmp;
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
	switch (head->operator){
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
	showExpression(head->next);
}

int solveExpression(Operation * head){
	NumberStack * headNumber = createNumberStack();
	OperatorStack * headOperator = createOperatorStack();
	if (head->solving != NULL){
		head->value = solveExpression(head->solving);
	}
	Operation *p;
	for (p = head; p != NULL; p = p->next){
		if (p->solving != NULL){
			p->value = solveExpression(p->solving);
		}	
		addToNumberStack(headNumber,p->value);
		addToOperatorStack(headOperator,p->operator);
		
	}
	detachValueOperatorStack(headOperator);
	int init = detachValueNumberStack(headNumber);
	while (headNumber->previous != NULL){
		Operator tmp = detachValueOperatorStack(headOperator);
		switch (tmp){
			case ADD:
				init += detachValueNumberStack(headNumber);
				break;
			case MUL:
				init *= detachValueNumberStack(headNumber);
				break;
			case DIV:
				init /= detachValueNumberStack(headNumber);
				break;
			case EXP:
				init = pow(detachValueNumberStack(headNumber),init);
				break;
			case EOE:
				printf("algo estranho aqui: %d\n",detachValueNumberStack(headNumber));
				break;
			default:
				printf("[default] algo estranho aqui: %d\n",detachValueNumberStack(headNumber));
				break;
		}
	}
	return init;
}


int main(){
	Operation * o = createHead(EOE);
	Operation *p;
	p = addOperationSolving(o, 10, ADD);  
	p = addOperation(p, 5, ADD);
	p = addOperation(p, 2, ADD);
	p = addOperation(p, -1, EOE);
	p = addOperationSolving(p, 3, ADD);
	p = addOperation(p, 5, EOE);

	showExpression(o);
	printf("\n");
	printf("resultado: %d\n",solveExpression(o));
	/*
	 * [HEAD] -> 10 + 5;
	 */
	
	return 0;
}
