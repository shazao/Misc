%{
  /* Usage: flex this-file. */
  #include "v.tab.h"
%}

%% 

module return kModule;
[_a-zA-Z][_a-z0-9A-Z]*    { yylval.s = strdup(yytext); return kVar; }
[ \t]+                    /* skip */
[,();]                    { return yytext[0]; }
\n
.                         yyerror("Invalid character.\n");

%%

int yywrap() {
  return 1;
}
