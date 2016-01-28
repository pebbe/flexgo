%{

#include <stdio.h>

#define YY_NO_INPUT
#define YY_NO_UNPUT

int yywrap(void) { 
    return 1; 
}

int i;

%}

%%

\0   fputs("<NUL>", yyout);

a\0b fputs("[a0b]", yyout);

a*\0+b*	{
	    for (i = yyleng - 1; i >= 0; i--)
	        if (yytext[i] == 0)
		    yytext[i] = '0';
	    fputc('(', yyout);
	    fputs(yytext, yyout);
	    fputc(')', yyout);
	}

%%

int main (int argc, char *argv []) { 
    yyin = fopen(argv[1], "r");
    yylex(); 
    fclose(yyin);
    return 0; 
} 
