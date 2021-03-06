/* -*-c-*- */
%{

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define YY_SKIP_YYWRAP
#ifdef yywrap
#  undef yywrap
#endif

int
    i,
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
        exit(1);
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

int
    count[YY_NUM_RULES+1];

#define YY_USER_ACTION count[yy_act]++;

%}

%s f1 f2

%%

<<EOF>>           {
                      fprintf(yyout, "<<EOF:%i>>\n", YY_START);
                      yyterminate();
                  }


bar               fprintf(yyout, "BAR");

\n                ECHO;


<INITIAL>{

(.|\n){4}         {
                      fprintf(yyout, "{1:%s}", yytext);
                      for (i = 0; i < 10000; i++)
                          unput (32 + i % 95);
                      BEGIN f1;
                  }

}

<f1,f2>{

foo               |

Foo               {
                      fprintf(yyout, "FOO");
                      yymore();
                  }

de[0-9]+          {
                      ECHO;
                      yyless(yyleng-2);
                  }

[aeiou]/0         fprintf(yyout, upstring(yytext));


q[0-9]+           {
                      for (i = 0; i < 100; i++)
                          if (input() == EOF)
                              yyterminate();
                  }

"FINAL PART"      {
                      ECHO;
                      fprintf(yyout, "\n>>>");
                      while ((i = input()) != EOF)
                          fputc(i, yyout);
                      fprintf(yyout, "<<<\n");
                      yyterminate();
                  }
}

%%

int main(int argc, char *argv []) {

    int
        i;

    for (i = 0; i < YY_NUM_RULES+1; i++)
        count[i] = 0;

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

    printf("\n\n");

    for (i = 1; i < YY_NUM_RULES+1; i++)
        printf("%4d\t%8d\n", i, count[i]);

    return 0;
}
