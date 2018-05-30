%{
  /* Usage: bison --defines this-file. */
  #include <stdio.h>
  void yyerror(char *);
%}

%start FileContent

%union {
  int i;
  char * s;
};
%token <s> kModule
%token <s> kVar

%%

FileContent: 
  | Design
  ;

Design: 
  Module
  | Design Module
  ;

Module: 
  kModule kVar '(' PortList ')' ';' { printf("module: %s\n", $2); free($2); }
  ;

PortList: 
  kVar                              { printf("Port: %s\n", $1); }
  | PortList ',' kVar               { printf("Port: %s\n", $3); }
  ;

%%

void yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
}

int main() {
  //yydebug = 1;
  yyparse();
  return 0;
}
