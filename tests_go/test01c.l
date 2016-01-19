%{

#include <errno.h>
#include <stdio.h>
#include <string.h>

int
    g_argc = 0;
char
    **g_argv;

int yywrap()
{
    if (g_argc) {
	fclose (yyin);
	g_argc--;
	g_argv++;
    }

    if (!g_argc)
	return 1;

    yyin = fopen(g_argv[0], "r");
    if (yyin == NULL) {
	fprintf(stderr, "Error opening file \"%s\": %s\n", g_argv[0], strerror(errno));
	return 1;
    }

    return 0;
}

%}


%%

foo|bar if (!strcmp (yytext, "foo")) { printf("FOO"); } else { printf("BAR"); }
C	printf("Go");

%%

int main(int argc, char *argv []) {

    if (argc > 1) {
	g_argc = argc - 1;
	g_argv = argv;
	g_argv++;

	yyin = fopen(g_argv[0], "r");
	if (yyin == NULL) {
	    fprintf(stderr, "Error opening file \"%s\": %s\n", g_argv[0], strerror(errno));
	    return 1;
	}
    }
    yylex();

    printf("Done\n");
    return 0;

}
