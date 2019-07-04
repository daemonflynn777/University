%{
#include <stdio.h>
int length=0;
%}
%%
P: S { printf ( "length: %d\n", length ); }
S: '('A')'S { $$=$4+1; length=$$; }
 | /*empty*/{ $$=0; }
A: '('A')'A {$$=$$+1;}
 | /*empty*/{ $$=0; }
%%
main() {
 printf ( "type a string, please: " );
 yyparse ();
}
yylex () {
 int c;
 c = getchar();
 if ( c=='\n' ) return 0;
 yylval = c;
 return c;
};
yyerror ( char *s ) {
 printf ( "Length eval: %s\n", s );
};
