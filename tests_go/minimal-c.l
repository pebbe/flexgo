%{

#include <stdio.h>

#define YY_NO_INPUT
#define YY_NO_UNPUT

int yywrap(void) { 
    return 1; 
}

%}

%%

%%

int main (int argc, char *argv []) { 
    yyin = fopen(argv[1], "r");
    yylex(); 
    fclose(yyin);
    return 0; 
} 
