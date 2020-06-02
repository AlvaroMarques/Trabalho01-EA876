

%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(char *c);
int yylex(void);
%}
%token START_JSON
%token STOP_JSON
%token TWO_DOTS
%token TEXT
%token NUM 
%token NUM_FLOAT
%token COMMA
%token START_LIST
%token END_LIST
%token EMPTY_LIST
%%

S: S FINAL_JSON {printf("VALIDO\n");}
	|;

FINAL_JSON: START_JSON JSON STOP_JSON {}
	|;

JSON: JSON COMMA JSON {}
	| TEXT TWO_DOTS FINAL_JSON {}
	| TEXT TWO_DOTS FINAL_LIST {}
	| TEXT TWO_DOTS EXP {};


LIST: FINAL_LIST {} 
	| EXP {} 
	| LIST COMMA LIST {}
	| EMPTY_LIST; 

FINAL_LIST: START_LIST LIST END_LIST {}; 

EXP: TEXT {}    
	| NUM {}
	| NUM_FLOAT {};


%%
/*

*/
void yyerror(char *s) {
	printf("INVALIDO\n");
}

int main() {
  yyparse();
    return 0;

}
