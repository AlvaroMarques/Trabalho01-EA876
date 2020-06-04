
%{
#include <stdlib.h>
void yyerror(char *);

#include "y.tab.h"

%}

%%

[0-9]+ {
  yylval = atoi(yytext);
  return Num;
}

[+] {
  return *yytext;
}

[*] {
  return *yytext;
}

[/] {
  return *yytext;
}

[\^] {
  return *yytext;
}

[(] {
  return *yytext;
}

[)] {
  return *yytext;
}

[[:space]] ;
[\n]+ ;
. ;

%%

int yywrap() {
  return 1;
}
