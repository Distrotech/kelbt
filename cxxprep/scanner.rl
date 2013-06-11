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

#include <iostream>
#include "scanner.h"

using std::cout;
using std::cerr;
using std::endl;

class Perfect_Hash
{
private:
	static inline unsigned int hash (const char *str, unsigned int len);

public:
	static struct Keyword *in_word_set (const char *str, unsigned int len);
};

%%{
	machine Scanner;

	action line_inc { line += 1; }

	# Floating literals.
	fract_const = digit* '.' digit+ | digit+ '.';
	exponent = [eE] [+\-]? digit+;
	float_suffix = [flFL];

	main := |*

	# Single and double literals.
	( 'L'? "'" ( [^'\\\n] | /\\./ )* "'" ) => { token( TK_SingleLit ); };
	( 'L'? '"' ( [^"\\\n] | /\\./ )* '"' ) => { token( TK_DoubleLit ); };

	# Identifiers
	( [a-zA-Z_] [a-zA-Z0-9_]* ) => { token( TK_Lookup ); };

	#
	# Qualified Identifiers
	#

	( '::' [a-zA-Z_] [a-zA-Z0-9_]* ) => { 
		token( TK_NameSep ); 

		/* It is okay to modify tokstart (but never tokend). */
		tokstart += 2;
		token( TK_Lookup, true ); 
	};

	( '::' '~' [a-zA-Z_] [a-zA-Z0-9_]* ) => { 
		token( TK_NameSep ); 
		token( '~' ); 

		/* It is okay to modify tokstart (but never tokend). */
		tokstart += 3;
		token( TK_Lookup, true ); 
	};

	( '->' [a-zA-Z_] [a-zA-Z0-9_]* ) => { 
		token( TK_Arrow ); 

		/* It is okay to modify tokstart (but never tokend). */
		tokstart += 2;
		token( TK_Lookup, true ); 
	};

	( '.' [a-zA-Z_] [a-zA-Z0-9_]* ) => { 
		token( '.' ); 

		/* It is okay to modify tokstart (but never tokend). */
		tokstart += 1;
		token( TK_Lookup, true ); 
	};


	# Floats.
	( fract_const exponent? float_suffix? |
		digit+ exponent float_suffix? ) => { token( TK_Float ); };

	# Integer decimal. Leading part buffered by float.
	( ( '0' | [1-9] [0-9]* ) [ulUL]{0,3} ) => { token( TK_IntegerDecimal ); };

	# Integer octal. Leading part buffered by float.
	( '0' [0-9]+ [ulUL]{0,2} ) => { token( TK_IntegerOctal ); };

	# Integer hex. Leading 0 buffered by float.
	( '0x' [0-9a-fA-F]+ [ulUL]{0,2} ) => { token( TK_IntegerHex ); };

	# Only buffer the second item, first buffered by symbol. */
	'::'  => { token( TK_NameSep ); };
	'=='  => { token( TK_EqualsEquals ); };
	'!='  => { token( TK_NotEquals ); };
	'&&'  => { token( TK_AndAnd ); };
	'||'  => { token( TK_OrOr ); };
	'*='  => { token( TK_MultAssign ); };
	'/='  => { token( TK_DivAssign ); };
	'%='  => { token( TK_PercentAssign ); };
	'+='  => { token( TK_PlusAssign ); };
	'-='  => { token( TK_MinusAssign ); };
	'&='  => { token( TK_AmpAssign ); };
	'^='  => { token( TK_CaretAssign ); };
	'|='  => { token( TK_BarAssign ); };
	'++'  => { token( TK_PlusPlus ); };
	'--'  => { token( TK_MinusMinus ); };
	'->'  => { token( TK_Arrow ); };
	'->*' => { token( TK_ArrowStar ); };
	'.*'  => { token( TK_DotStar ); };
	'...' => { token( TK_DotDotDot ); };

	# Single char symbols.
	( punct - [#_"'{}] ) => { token( *tokstart ); };

	# Handling the curly blocks in the scanner rather than lookup is dependent
	# on the token sequence being flushed out to the parser immediately upon
	# seeing '{' so that the parser can decide if we ignore the block.

	# Curly Open.
	'{' => {
		token( '{' );
	};

	# Curly close.
	'}' => {
		token( '}' ); 
	};

	# Preprocessor line.
	'#' [^\n]* '\n' => { line++; };

	action leader {
		leaderBuf.append( tokstart, tokend-tokstart );
	}

	# Comments and whitespace.
	( '/*' (any | '\n' @line_inc)* :>> '*/' ) => leader;
	( '//' any* :> '\n' @line_inc ) => leader;
	( any - 10 - 33..126 )+ => leader;

	# Newline handling.
	'\n' => {
		/* Process the newline first. */
		line++;
		leaderBuf.append( '\n' );
	};

	*|;
}%%

%% write data;

void Scanner::init()
{
	%% write init;

	line = 1;
	parser.init();
}

int Scanner::execute( char *data, int len )
{
	char *p = data;
	char *pe = data + len;
	%% write exec;
	if ( cs == Scanner_error )
		return -1;
	else if ( cs >= Scanner_first_final )
		return 1;
	return 0;
}

void Scanner::token( int tok, bool qual )
{
	int len = tokend - tokstart;

	/* Translate keywords. */
	if ( tok == TK_Lookup ) {
		Keyword *key = Perfect_Hash::in_word_set( tokstart, len );
		if ( key != 0 )
			tok = key->id;
	}

#ifdef LOG_TOKENS
	cerr << "token: line: " << line << " id: " << tok << " data: ";
	cerr.write( tokstart, len );
	cerr << "  -> sending to parser" << endl;
#endif

	char *data = 0;
	switch ( tok ) {
		case TK_Lookup: 
		case TK_Integer:
		case TK_DoubleLit: 
		case TK_SingleLit: 
		case TK_Float: 
		case TK_IntegerOctal:
		case TK_IntegerDecimal:
		case TK_IntegerHex:
			data = strAlloc.allocate( tokstart, len );
			break;
	}
	
	char *leader = 0;
	if ( leaderBuf.length > 0 ) {
		leader = strAlloc.allocate( leaderBuf.data, leaderBuf.length );
		leaderBuf.clear();
	}

	int res = parser.parseToken( tok, qual, data, leader );
	if ( res < 0 )
		exit(1);
}

void Scanner::finish()
{
	char eofChar = 0;
	execute( &eofChar, 1 );

	/* Send the eof char to the parser. */
	parser.parseToken( _eof, false, 0, 0 );
	int result = parser.errCount;
	
	if ( result == 0 && leaderBuf.length > 0 )
		cout.write( leaderBuf.data, leaderBuf.length );
}
