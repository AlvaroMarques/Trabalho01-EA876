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

typedef struct NumberList {
	struct NumberList * next;
	struct NumberList * previous;
	int value;
} NumberList;

NumberList * createNumberPointer(){
	NumberList * head = malloc(sizeof(NumberList));
	head->value = -1;
	head->previous = NULL;
	head->next=NULL;
	return head;
}

void addToNumberList(NumberList * head, int value){
	NumberList * new = malloc(sizeof(NumberList));	
	new->value  = value;
	new->previous = head->previous;
	if (head->previous != NULL)
		head->previous->next = new;
	head->previous = new;
}

int detachValueNumberList (NumberList * head){
	NumberList * freedom = head->previous;
	int tmp = freedom->value;
	head->previous = freedom->previous;
	free(freedom);
	return tmp;
}
int gotoPreviousNumberList(NumberList *head){
	if (head == NULL) return EOE;
	int ret = head->previous->value; 
	head->previous = head->previous->previous;
	return ret;
}


typedef struct OperatorList {
	struct OperatorList * next;
	struct OperatorList * previous;
	int value;
} OperatorList;

OperatorList * createOperatorPointer(){
	OperatorList * head = malloc(sizeof(OperatorList));
	head->value = EOE;
	head->previous = NULL;
	head->next=NULL;
	return head;
}

void addToOperatorList(OperatorList * head, Operator value){
	OperatorList * new = malloc(sizeof(OperatorList));	
	new->value  = value;
	new->previous = head->previous;
	if (head->previous != NULL)
		head->previous->next = new;
	head->previous = new;
}

Operator gotoPreviousOperatorList(OperatorList *head){
	if (head == NULL) return EOE;
	Operator ret = head->previous->value; 
	head->previous = head->previous->previous;
	return ret;
}

int detachValueOperatorList (OperatorList * head){
	OperatorList * freedom = head->previous;
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
	NumberList * headNumber = createNumberPointer();
	OperatorList * headOperator = createOperatorPointer();
	if (head->solving != NULL){
		head->value = solveExpression(head->solving);
	}
	Operation *p;
	for (p = head; p != NULL; p = p->next){
		if (p->solving != NULL){
			p->value = solveExpression(p->solving);
		}	
		addToNumberList(headNumber,p->value);
		addToOperatorList(headOperator,p->operator);
	}
	detachValueOperatorList(headOperator);
	int init = detachValueNumberList(headNumber);
	while (headNumber->previous != NULL){
		Operator tmp = detachValueOperatorList(headOperator);
		switch (tmp){
			case ADD:
				init += detachValueNumberList(headNumber);
				break;
			case MUL:
				init *= detachValueNumberList(headNumber);
				break;
			case DIV:
				init /= detachValueNumberList(headNumber);
				break;
			case EXP:
				init = pow(detachValueNumberList(headNumber),init);
				break;
			case EOE:
				printf("algo estranho aqui: %d\n",detachValueNumberList(headNumber));
				break;
			default:
				printf("[default] algo estranho aqui: %d\n",detachValueNumberList(headNumber));
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
