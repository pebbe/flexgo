%{

#include <errno.h>
#include <stdio.h>
#include <string.h>

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

%}

%%

foo     {
          ECHO;
          yymore();
        }

bar     {
          ECHO;
          int c = input();
          fprintf(yyout, "%c%c%c", c, c, c);
        }

\[baz\] {
          ECHO;
          ECHO;
        }

baz    {
         int i;
         /* Copy yytext because unput() trashes yytext */
         char *yycopy = strdup( yytext );
         unput( ']' );
         for ( i = yyleng - 1; i >= 0; --i )
             unput( yycopy[i] );
         unput( '[' );
         free( yycopy );
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
