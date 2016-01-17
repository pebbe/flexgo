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

foo|bar if (!strcmp (yytext, "foo")) { printf("FOO"); } else { printf("BAR"); }
C	printf("Go");

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
