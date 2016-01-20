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
    toggle = 1;

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


%x _str

%%

<INITIAL>{

"\""    { printf("<<");
          BEGIN _str;
        }

^.	printf("%c", yytext[0] - ((yytext[0] >= 'a' && yytext[0] <= 'z') ? 32 : 0));

foo/bar	{
		toggle = 1 - toggle;
		if (toggle)
			printf("FOO");
		else
			REJECT;
	}

}

<_str>{

[^\"\\]+  ECHO;

"\""    { printf(">>");
          BEGIN INITIAL;
        }

\\.	printf("%c", yytext[1]);

<<EOF>>	{
	  printf("Closing quote is missing\n");
	  yyterminate();
	}

}

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
