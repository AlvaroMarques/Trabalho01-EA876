
%{
#include <stdlib.h>
void yyerror(char *);

#include "y.tab.h"

%}


%%
 
%%

int yywrap() {
  return 1;
}
