/*
 *  Copyright 2004, 2005 Adrian Thurston <thurston@complang.org>
 */

/*  This file is part of C++ Pre-Parser.
 *
 *  C++ Pre-Parser is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  C++ Pre-Parser is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with C++ Pre-Parser; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

%{
#include <stdio.h>
#include <string.h>

%}

%%

	/* Find directives to pass through */
\/\*\ CXXPREP\ #.*\*\/ {
	yytext[ strlen(yytext)-2 ] = 0;
	fputs( yytext+11, stdout );
}

\[di\ [^\]]*\] {
	yytext[ strlen(yytext)-1 ] = 0;
	fputs( yytext+4, stdout );
}

\[cn\ [^\]]*\] {
	yytext[ strlen(yytext)-1 ] = 0;
	fputs( yytext+4, stdout );
}

\[en\ [^\]]*\] {
	yytext[ strlen(yytext)-1 ] = 0;
	fputs( yytext+4, stdout );
}

\[nn\ [^\]]*\] {
	yytext[ strlen(yytext)-1 ] = 0;
	fputs( yytext+4, stdout );
}

	/* Pass comments and literals throuh. */
\/\/.*\n fputs( yytext, stdout );
\/\*.*\*\/ fputs( yytext, stdout );
'([^'\\\n]|\\.)*' fputs( yytext, stdout );
\"([^"\\\n]|\\.)*\" fputs( yytext, stdout );

\$ /* Ignore */

	/* Catch the qualifier sep so we don't make a mistake on C::~C(); */
"::" fputs( "::", stdout );

"[[" putchar( '[' );
"]]" putchar( ']' );
"({" putchar( '(' );
"})" putchar( ')' );
"(|" putchar( '(' );
"|)" putchar( ')' );
"<{" putchar( '<' );
"}>" putchar( '>' );
"<|" putchar( '<' );
"|>" putchar( '>' );
"<&>" putchar( '&' );
"<*>" putchar( '*' );
":/" putchar( ':' );
":+" putchar( ':' );
":-" putchar( ':' );
":~" putchar( ':' );
":|" putchar( ':' );

%%

int yywrap()
{
	/* Once processessing of the input is done, no more. */
	return 1;
}

int main()
{
	yylex();
	return 0;
};


