%{

#include <stdio.h>

int yywrap()
{
    return 1;
}

%}


%%

C	printf("Go");

%%

int main() {
    yylex();
    return 0;
}
