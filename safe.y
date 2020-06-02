

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
%%

S: S FINAL_JSON {printf("VALIDO\n");}
	|;

FINAL_JSON: START_JSON JSON STOP_JSON {printf("aqui\n");}
	|;

JSON: JSON COMMA JSON {}
	| TEXT TWO_DOTS FINAL_JSON {}
	| TEXT TWO_DOTS EXP {printf("oi\n");};

EXP: TEXT {printf("exp\n");}    
	| NUM {printf("exp\n");}
	| NUM_FLOAT {printf("exp\n");};


%%
/*
	| TEXT TWO_DOTS LIST  {}
LIST: START_LIST EXP COMMA LIST END_LIST{printf("hello\n");} 
	| START_LIST EXP END_LIST
	| START_LIST END_LIST;
*/
void yyerror(char *s) {
	printf("INVALIDO\n");
}

int main() {
  yyparse();
    return 0;

}
