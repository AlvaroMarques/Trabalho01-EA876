
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

<<EOF>> {
  return EndOF;
}
[[:space]] ;
[\n]+ ;
. ;

%%

int yywrap() {
  return 1;
}
