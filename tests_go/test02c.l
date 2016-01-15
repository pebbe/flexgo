%{

#include <errno.h>
#include <stdio.h>
#include <string.h>

int yywrap()
{
    return 1;
}

%}


%%

^.	printf("%c", yytext[0] - ((yytext[0] >= 'a' && yytext[0] <= 'z') ? 32 : 0));

%%

int main(int argc, char *argv []) {
    int
	i;
    if (argc > 1) {
	for (i = 1; i < argc; i++) {
	    yyin = fopen(argv[i], "r");
	    if (yyin == NULL) {
		fprintf(stderr, "Error opening file \"%s\": %s\n", argv[i], strerror(errno));
		return 1;
	    }
	    yyrestart(yyin);
	    yylex();
	    fclose(yyin);
	}
    } else {
	yylex();
    }
    printf("Done\n");
    return 0;
}
