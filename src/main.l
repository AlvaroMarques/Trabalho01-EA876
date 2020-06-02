
%{
#include <stdlib.h>
void yyerror(char *);

#include "y.tab.h"

%}


%%

[\{] {
	return START_JSON;
}

[\}] {
	return STOP_JSON;
}

[:] {
	return TWO_DOTS;
}

[,] {
	return COMMA;
}

[\[] {
	return START_LIST;
}

[\[][\]] {
	return EMPTY_LIST;
}

[\]] {
	return END_LIST;
}

[0-9]+[.][0-9]+ {
	return NUM_FLOAT;
}
[0-9]+ {
	return NUM;
}
\"[A-Za-z0-9\.\,\-\:\;\ ]+\" {
	return TEXT;
}

[[:space]] ;
[\n]+ ;
. ;
 
%%

int yywrap() {
  return 1;
}
