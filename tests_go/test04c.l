%{

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define YY_NO_INPUT
#define YY_NO_UNPUT
#define YY_SKIP_YYWRAP
#ifdef yywrap
#  undef yywrap
#endif

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

char *upstring(char *s) {
    int i;
        for (i = 0; s[i]; i++)
            if (s[i] >= 'a' && s[i] <= 'z')
                s[i] -= 32;
    return s;
}

%}


%%

foo/bar		{ printf("%sB", upstring(yytext)); }
fo+/[a-np-z]+	{ printf("%sX", upstring(yytext)); }

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
