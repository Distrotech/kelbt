
#line 1 "klscan.rl"
/*
 *  Copyright 2006-2009 Adrian Thurston <thurston@complang.org>
 */

/*  This file is part of Kelbt.
 *
 *  Kelbt is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  Kelbt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with Kelbt; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include <iostream>
#include <fstream>
#include <string.h>

#include "kelbt.h"
#include "klparse.h"

using std::ifstream;
using std::istream;
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;


#line 40 "klscan.cpp"
static const int section_parser_start = 7;
static const int section_parser_first_final = 7;
static const int section_parser_error = 0;

static const int section_parser_en_main = 7;


#line 39 "klscan.rl"


struct Scanner
{
	Scanner( const char *fileName, istream &input, 
			ParserDict &parserDict, int include_depth ) : 
		fileName(fileName), input(input), 
		parserDict(parserDict), include_depth(include_depth),
		parser(0), curline(1)
	{
		
#line 60 "klscan.cpp"
	{
	cs = section_parser_start;
	}

#line 50 "klscan.rl"
	}
	
	void startSection( bool singleLineSpec );
	void endSection();

	void do_scan();
	void try_token( int tokid );
	void try_token( int tokid, char *start, char *end );

	ifstream *tryOpenInclude( char **pathChecks, long &found );
	char **makeIncludePathChecks( const char *thisFileName, const char *fileName );

	/* Open an error message with the file name and line. */
	ostream &error()
	{
		/* Keep the error count. */
		gblErrorCount += 1;
	
		cerr << fileName << ":" << curline << ": ";
		return cerr;
	}

	const char *fileName;
	istream &input;
	ParserDict &parserDict;
	int include_depth;
	Parser *parser;
	int curline, inline_start_line;
	bool singleLineSpec;

	/* Write statements seen. */
	bool writeInstanceData;
	bool writeTokenDefs;
	bool writeTypes; 
	bool writeData;
	bool writeInit;
	bool writeExec;
	bool writeFinish;

	/* For section parser. */
	int cs;
	char *captured;
};

void Scanner::startSection( bool singleLineSpec )
{
	this->singleLineSpec = singleLineSpec;
	writeInstanceData = false;
	writeTokenDefs = false;
	writeTypes = false;
	writeData = false;
	writeInit = false;
	writeExec = false;
	writeFinish = false;
}

void Scanner::endSection()
{
	if ( !::generateGraphviz ) {
		if ( writeInstanceData )
			parser->pd.generateInstanceData();
		if ( writeTokenDefs )
			parser->pd.generateTokenDefs();
		if ( writeTypes )
			parser->pd.generateTypes();
		if ( writeInit )
			parser->pd.generateInit();
		if ( writeData )
			parser->pd.generateData();
		if ( writeExec )
			parser->pd.generateExec();
		if ( writeFinish )
			parser->pd.generateFinish();
		
		if ( ! inhibitLineDirectives )
			*outStream << "#line " << curline << " \"" << fileName << "\"\n";
	}
	
	/* This causes us to use the parser with the last parser definition when
	 * generating graphviz code. */
	graphvizGenParser = parser;
}

void Scanner::try_token( int tokid )
{
	try_token( tokid, 0, 0 );
}

bool isAbsolutePath( const char *path )
{
	return path[0] == '/';
}

ifstream *Scanner::tryOpenInclude( char **pathChecks, long &found )
{
	char **check = pathChecks;
	ifstream *inFile = new ifstream;
	
	while ( *check != 0 ) {
		inFile->open( *check );
		if ( inFile->is_open() ) {
			found = check - pathChecks;
			return inFile;
		}
		check += 1;
	}

	found = -1;
	delete inFile;
	return 0;
}

char **Scanner::makeIncludePathChecks( const char *thisFileName, const char *fileName )
{
	char **checks = 0;
	long nextCheck = 0;
	char *data = strdup(fileName);
	long length = strlen(fileName);

	/* Absolute path? */
	if ( isAbsolutePath( data ) ) {
		checks = new char*[2];
		checks[nextCheck++] = data;
	}
	else {
		/* Search from the the location of the current file. */
		checks = new char *[2 + includePaths.length()];
		const char *lastSlash = strrchr( thisFileName, '/' );
		if ( lastSlash == 0 )
			checks[nextCheck++] = data;
		else {
			long givenPathLen = (lastSlash - thisFileName) + 1;
			long checklen = givenPathLen + length;
			char *check = new char[checklen+1];
			memcpy( check, thisFileName, givenPathLen );
			memcpy( check+givenPathLen, data, length );
			check[checklen] = 0;
			checks[nextCheck++] = check;
		}

		/* Search from the include paths given on the command line. */
		for ( ArgsVector::Iter incp = includePaths; incp.lte(); incp++ ) {
			long pathLen = strlen( *incp );
			long checkLen = pathLen + 1 + length;
			char *check = new char[checkLen+1];
			memcpy( check, *incp, pathLen );
			check[pathLen] = '/';
			memcpy( check+pathLen+1, data, length );
			check[checkLen] = 0;
			checks[nextCheck++] = check;
		}
	}

	checks[nextCheck] = 0;
	return checks;
}


void Scanner::try_token( int tokid, char *start, char *end )
{
	char *tokdata = 0;
	int *p = &tokid, *pe = &tokid + 1;

	if ( start != 0 ) {
		int len = end-start;
		tokdata = new char[len+1];
		memcpy( tokdata, start, len );
		tokdata[len] = 0;
	}


#line 237 "klscan.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 228 "klscan.rl"
	{
		//cout << "PARSER STATEMENT" << endl;

		char *parserName = captured;
		ParserDictEl *pdEl = parserDict.find( parserName );
		if ( pdEl != 0 ) {
			//cout << "USING EXISTING PARSER" << endl;
			delete[] parserName;
		}
		else {
			//cout << "CREATING NEW PARSER" << endl;
			pdEl = new ParserDictEl;
			pdEl->name = parserName;
			pdEl->parser = new Parser( fileName, parserName, *outStream );
			pdEl->parser->init();
			parserDict.insert( pdEl );
		}

		parser = pdEl->parser;
	}
	goto st7;
tr4:
#line 250 "klscan.rl"
	{
		/* Make the list of files to try. */
		char **checks = makeIncludePathChecks( this->fileName, captured );

		/* Open the input file for reading. */
		long found = 0;
		ifstream *inFile = tryOpenInclude( checks, found );

		/* Open the input file for reading. */
		if ( inFile == 0 )
			error() << "include: could not open " << captured << " for reading" << endl;
		else {
			Scanner subScanner( checks[found], *inFile, parserDict, include_depth+1 );
			subScanner.do_scan();
			delete inFile;
		}
	}
	goto st7;
tr6:
#line 269 "klscan.rl"
	{
		if ( include_depth == 0 ) {
			char *writeArg = captured;
			if ( strcmp( writeArg, "instance_data" ) == 0 )
				writeInstanceData = true;
			else if ( strcmp( writeArg, "token_defs" ) == 0 )
				writeTokenDefs = true;
			else if ( strcmp( writeArg, "types" ) == 0 )
				writeTypes = true;
			else if ( strcmp( writeArg, "data" ) == 0 )
				writeData = true;
			else if ( strcmp( writeArg, "init" ) == 0 )
				writeInit = true;
			else if ( strcmp( writeArg, "exec" ) == 0 )
				writeExec = true;
			else if ( strcmp( writeArg, "finish" ) == 0 )
				writeFinish = true;
		}
	}
	goto st7;
tr7:
#line 290 "klscan.rl"
	{
		if ( parser == 0 )
			error() << "no section name and no previously named section" << endl;
		else {
			int line = TK_Inline ? inline_start_line : curline;
			int result = parser->token( line, tokid, tokdata );
			if ( result < 0 )
				error() << "parse error on or near this line" << endl;
		}
	}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 325 "klscan.cpp"
	switch( (*p) ) {
		case 128: goto st1;
		case 129: goto st3;
		case 130: goto st5;
	}
	goto tr7;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) == 143 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 301 "klscan.rl"
	{ captured = tokdata; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 350 "klscan.cpp"
	if ( (*p) == 59 )
		goto tr2;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 144 )
		goto tr3;
	goto st0;
tr3:
#line 302 "klscan.rl"
	{ captured = tokdata; }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 369 "klscan.cpp"
	if ( (*p) == 59 )
		goto tr4;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 143 )
		goto tr5;
	goto st0;
tr5:
#line 301 "klscan.rl"
	{ captured = tokdata; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 388 "klscan.cpp"
	if ( (*p) == 59 )
		goto tr6;
	goto st0;
	}
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 318 "klscan.rl"

}


#line 460 "klscan.rl"



#line 414 "klscan.cpp"
static const int klscan_start = 21;
static const int klscan_first_final = 21;
static const int klscan_error = 0;

static const int klscan_en_inline_code = 30;
static const int klscan_en_access_stmt = 40;
static const int klscan_en_parser_def = 42;
static const int klscan_en_main = 21;


#line 463 "klscan.rl"

void Scanner::do_scan()
{
	int bufsize = 8;
	char *buf = new char[bufsize];
	char *ts, *te;
	int cs, act, have = 0;
	int curly_count = 0;
	char *inline_start = 0;
	char *litstart = 0, *litend = 0;
	char *identstart = 0, *identend = 0;
	bool execute = true;

	
#line 440 "klscan.cpp"
	{
	cs = klscan_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 477 "klscan.rl"

	while ( execute ) {
		char *p = buf + have;
		int space = bufsize - have;

		if ( space == 0 ) {
			/* We filled up the buffer trying to scan a token. Grow it. */
			bufsize = bufsize * 2;
			char *newbuf = new char[bufsize];
			//cout << "FULL BUFFER, NEW SIZE: " << bufsize << endl;

			/* Recompute p and space. */
			p = newbuf + have;
			space = bufsize - have;

			/* Patch up pointers possibly in use. */
			if ( ts != 0 )
				ts = newbuf + ( ts - buf );
			if ( inline_start != 0 )
				inline_start = newbuf + ( inline_start - buf );
			te = newbuf + ( te - buf );
			litstart = newbuf + ( litstart - buf );
			litend = newbuf + ( litend - buf );
			identstart = newbuf + ( identstart - buf );
			identend = newbuf + ( identend - buf );

			/* Copy the new buffer in. */
			memcpy( newbuf, buf, have );
			delete[] buf;
			buf = newbuf;
		}

		input.read( p, space );
		int len = input.gcount();
		char *pe = p + len;

		/* Check for EOF. */
		char *eof = 0;
	 	if ( len == 0 ) {
			eof = pe;
			execute = false;
		}

		
#line 493 "klscan.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 436 "klscan.rl"
	{{p = ((te))-1;}{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr2:
#line 436 "klscan.rl"
	{te = p+1;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr9:
#line 436 "klscan.rl"
	{te = p+1;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr11:
#line 328 "klscan.rl"
	{ curline++; }
#line 436 "klscan.rl"
	{te = p+1;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr33:
#line 436 "klscan.rl"
	{te = p+1;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr34:
#line 456 "klscan.rl"
	{te = p+1;}
	goto st21;
tr43:
#line 436 "klscan.rl"
	{te = p;p--;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr44:
#line 436 "klscan.rl"
	{te = p;p--;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr46:
#line 447 "klscan.rl"
	{te = p;p--;{
			startSection( true );
			{goto st42;}
		}}
	goto st21;
tr47:
#line 451 "klscan.rl"
	{te = p+1;{ 
			startSection( false );
			{goto st42;}
		}}
	goto st21;
tr48:
#line 436 "klscan.rl"
	{te = p;p--;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
tr49:
#line 436 "klscan.rl"
	{te = p;p--;{
		if ( !generateGraphviz && gblErrorCount == 0 && include_depth == 0 )
			outStream->write( ts, te-ts );
	}}
	goto st21;
st21:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 1 "NONE"
	{ts = p;}
#line 590 "klscan.cpp"
	switch( (*p) ) {
		case 0: goto tr34;
		case 9: goto st22;
		case 10: goto tr36;
		case 13: goto st22;
		case 32: goto st22;
		case 34: goto tr37;
		case 37: goto st24;
		case 39: goto tr39;
		case 47: goto tr40;
		case 95: goto st29;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st28;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st29;
	} else
		goto st29;
	goto tr33;
tr36:
#line 328 "klscan.rl"
	{ curline++; }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 620 "klscan.cpp"
	switch( (*p) ) {
		case 9: goto st22;
		case 10: goto tr36;
		case 13: goto st22;
		case 32: goto st22;
	}
	goto tr43;
tr37:
#line 1 "NONE"
	{te = p+1;}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 636 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr44;
		case 13: goto tr44;
		case 34: goto tr2;
		case 92: goto st2;
	}
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 10: goto tr0;
		case 13: goto tr0;
		case 34: goto tr2;
		case 92: goto st2;
	}
	goto st1;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr0;
	goto st1;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 37 )
		goto st25;
	goto tr44;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 123 )
		goto tr47;
	goto tr46;
tr39:
#line 1 "NONE"
	{te = p+1;}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 684 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr44;
		case 13: goto tr44;
		case 39: goto tr2;
		case 92: goto st4;
	}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 10: goto tr0;
		case 13: goto tr0;
		case 39: goto tr2;
		case 92: goto st4;
	}
	goto st3;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr0;
	goto st3;
tr40:
#line 1 "NONE"
	{te = p+1;}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 718 "klscan.cpp"
	switch( (*p) ) {
		case 42: goto st5;
		case 47: goto st7;
	}
	goto tr44;
tr7:
#line 328 "klscan.rl"
	{ curline++; }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 732 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr7;
		case 42: goto st6;
	}
	goto st5;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 10: goto tr7;
		case 42: goto st6;
		case 47: goto tr9;
	}
	goto st5;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 10 )
		goto tr11;
	goto st7;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto tr48;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 95 )
		goto st29;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st29;
	} else
		goto st29;
	goto tr49;
tr12:
#line 368 "klscan.rl"
	{{p = ((te))-1;}}
	goto st30;
tr14:
#line 347 "klscan.rl"
	{te = p+1;}
	goto st30;
tr21:
#line 346 "klscan.rl"
	{te = p+1;}
	goto st30;
tr23:
#line 328 "klscan.rl"
	{ curline++; }
#line 346 "klscan.rl"
	{te = p+1;}
	goto st30;
tr50:
#line 368 "klscan.rl"
	{te = p+1;}
	goto st30;
tr60:
#line 350 "klscan.rl"
	{te = p+1;{ curly_count += 1; }}
	goto st30;
tr61:
#line 352 "klscan.rl"
	{te = p+1;{ 
			if ( --curly_count == 0 ) {
				/* Send the entire block and free the inline_start pointer. */
				try_token( TK_Inline, inline_start, te );
				inline_start = 0;
				{goto st42;}
			}
		}}
	goto st30;
tr62:
#line 348 "klscan.rl"
	{te = p;p--;}
	goto st30;
tr63:
#line 368 "klscan.rl"
	{te = p;p--;}
	goto st30;
tr64:
#line 361 "klscan.rl"
	{te = p+1;{
			if ( inline_start < ts )
				try_token( TK_Inline, inline_start, ts );
			try_token( TK_Reference, ts, te );
			inline_start = te;
		}}
	goto st30;
tr66:
#line 361 "klscan.rl"
	{te = p;p--;{
			if ( inline_start < ts )
				try_token( TK_Inline, inline_start, ts );
			try_token( TK_Reference, ts, te );
			inline_start = te;
		}}
	goto st30;
tr67:
#line 345 "klscan.rl"
	{te = p;p--;}
	goto st30;
tr68:
#line 344 "klscan.rl"
	{te = p;p--;}
	goto st30;
st30:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 1 "NONE"
	{ts = p;}
#line 856 "klscan.cpp"
	switch( (*p) ) {
		case 9: goto st31;
		case 10: goto tr52;
		case 13: goto st31;
		case 32: goto st31;
		case 34: goto tr53;
		case 36: goto st33;
		case 39: goto tr55;
		case 47: goto tr56;
		case 64: goto st38;
		case 95: goto st39;
		case 123: goto tr60;
		case 125: goto tr61;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st37;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st39;
	} else
		goto st39;
	goto tr50;
tr52:
#line 328 "klscan.rl"
	{ curline++; }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 888 "klscan.cpp"
	switch( (*p) ) {
		case 9: goto st31;
		case 10: goto tr52;
		case 13: goto st31;
		case 32: goto st31;
	}
	goto tr62;
tr53:
#line 1 "NONE"
	{te = p+1;}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 904 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr63;
		case 13: goto tr63;
		case 34: goto tr14;
		case 92: goto st9;
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 10: goto tr12;
		case 13: goto tr12;
		case 34: goto tr14;
		case 92: goto st9;
	}
	goto st8;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 10 )
		goto tr12;
	goto st8;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 36 )
		goto tr64;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st34;
	goto tr63;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st34;
	goto tr66;
tr55:
#line 1 "NONE"
	{te = p+1;}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 954 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr63;
		case 13: goto tr63;
		case 39: goto tr14;
		case 92: goto st11;
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 10: goto tr12;
		case 13: goto tr12;
		case 39: goto tr14;
		case 92: goto st11;
	}
	goto st10;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 10 )
		goto tr12;
	goto st10;
tr56:
#line 1 "NONE"
	{te = p+1;}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 988 "klscan.cpp"
	switch( (*p) ) {
		case 42: goto st12;
		case 47: goto st14;
	}
	goto tr63;
tr19:
#line 328 "klscan.rl"
	{ curline++; }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 1002 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr19;
		case 42: goto st13;
	}
	goto st12;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 10: goto tr19;
		case 42: goto st13;
		case 47: goto tr21;
	}
	goto st12;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 10 )
		goto tr23;
	goto st14;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st37;
	goto tr67;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 64 )
		goto tr64;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st34;
	goto tr63;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 95 )
		goto st39;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st39;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st39;
	} else
		goto st39;
	goto tr68;
tr71:
#line 373 "klscan.rl"
	{te = p+1;{ 
			try_token( ';' ); 
			{goto st42;}
		}}
	goto st40;
tr72:
#line 372 "klscan.rl"
	{te = p;p--;{ try_token( TK_AccessData, ts, te ); }}
	goto st40;
st40:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1 "NONE"
	{ts = p;}
#line 1075 "klscan.cpp"
	switch( (*p) ) {
		case 0: goto st0;
		case 59: goto tr71;
	}
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 0: goto tr72;
		case 59: goto tr72;
	}
	goto st41;
st0:
cs = 0;
	goto _out;
tr24:
#line 433 "klscan.rl"
	{{p = ((te))-1;}{ try_token( *ts, 0, 0 ); }}
	goto st42;
tr26:
#line 407 "klscan.rl"
	{te = p+1;{ try_token( TK_String, ts+1, te-1 ); }}
	goto st42;
tr29:
#line 328 "klscan.rl"
	{ curline++; }
#line 410 "klscan.rl"
	{te = p+1;}
	goto st42;
tr30:
#line 406 "klscan.rl"
	{te = p+1;{ try_token( TK_Literal, ts+1, te-1 ); }}
	goto st42;
tr32:
#line 412 "klscan.rl"
	{te = p+1;{ 
			endSection();
			{goto st21;}
		}}
	goto st42;
tr73:
#line 433 "klscan.rl"
	{te = p+1;{ try_token( *ts, 0, 0 ); }}
	goto st42;
tr75:
#line 328 "klscan.rl"
	{ curline++; }
#line 419 "klscan.rl"
	{te = p+1;{
			if ( singleLineSpec ) {
				endSection();
				{goto st21;}
			}
		}}
	goto st42;
tr91:
#line 426 "klscan.rl"
	{te = p+1;{ 
			inline_start_line = curline;
			inline_start = ts;
			curly_count = 1; 
			{goto st30;}
		}}
	goto st42;
tr93:
#line 417 "klscan.rl"
	{te = p;p--;}
	goto st42;
tr94:
#line 433 "klscan.rl"
	{te = p;p--;{ try_token( *ts, 0, 0 ); }}
	goto st42;
tr96:
#line 408 "klscan.rl"
	{te = p;p--;{ try_token( TK_Number, ts, te ); }}
	goto st42;
tr97:
#line 1 "NONE"
	{	switch( act ) {
	case 12:
	{{p = ((te))-1;} try_token( KW_Commit ); }
	break;
	case 13:
	{{p = ((te))-1;} try_token( KW_Try ); }
	break;
	case 14:
	{{p = ((te))-1;} try_token( KW_Undo ); }
	break;
	case 15:
	{{p = ((te))-1;} try_token( KW_Final ); }
	break;
	case 16:
	{{p = ((te))-1;} try_token( KW_Translate ); }
	break;
	case 17:
	{{p = ((te))-1;} try_token( KW_Token ); }
	break;
	case 18:
	{{p = ((te))-1;} try_token( KW_NonTerm ); }
	break;
	case 19:
	{{p = ((te))-1;} try_token( KW_Uses ); }
	break;
	case 20:
	{{p = ((te))-1;} try_token( KW_Type ); }
	break;
	case 21:
	{{p = ((te))-1;} try_token( KW_Parser ); }
	break;
	case 22:
	{{p = ((te))-1;} try_token( KW_Include ); }
	break;
	case 23:
	{{p = ((te))-1;} try_token( KW_Pri ); }
	break;
	case 24:
	{{p = ((te))-1;} try_token( KW_Write ); }
	break;
	case 25:
	{{p = ((te))-1;} try_token( KW_Class ); }
	break;
	case 26:
	{{p = ((te))-1;} try_token( KW_Shortest ); }
	break;
	case 28:
	{{p = ((te))-1;} try_token( TK_Word, ts, te ); }
	break;
	}
	}
	goto st42;
tr98:
#line 405 "klscan.rl"
	{te = p;p--;{ try_token( TK_Word, ts, te ); }}
	goto st42;
tr104:
#line 397 "klscan.rl"
	{te = p;p--;{ 
			try_token( KW_Access );
			{goto st40;}
		}}
	goto st42;
st42:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 1 "NONE"
	{ts = p;}
#line 1227 "klscan.cpp"
	switch( (*p) ) {
		case 9: goto st43;
		case 10: goto tr75;
		case 13: goto st43;
		case 32: goto st43;
		case 34: goto tr76;
		case 35: goto tr77;
		case 39: goto tr78;
		case 95: goto tr80;
		case 97: goto st49;
		case 99: goto st56;
		case 102: goto st64;
		case 105: goto st68;
		case 110: goto st74;
		case 112: goto st80;
		case 115: goto st86;
		case 116: goto st93;
		case 117: goto st106;
		case 119: goto st111;
		case 123: goto tr91;
		case 125: goto tr92;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr73;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 9: goto st43;
		case 13: goto st43;
		case 32: goto st43;
	}
	goto tr93;
tr76:
#line 1 "NONE"
	{te = p+1;}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1277 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr94;
		case 13: goto tr94;
		case 34: goto tr26;
		case 92: goto st16;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 10: goto tr24;
		case 13: goto tr24;
		case 34: goto tr26;
		case 92: goto st16;
	}
	goto st15;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 10 )
		goto tr24;
	goto st15;
tr77:
#line 1 "NONE"
	{te = p+1;}
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 1311 "klscan.cpp"
	if ( (*p) == 10 )
		goto tr29;
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 10 )
		goto tr29;
	goto st17;
tr78:
#line 1 "NONE"
	{te = p+1;}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 1330 "klscan.cpp"
	switch( (*p) ) {
		case 10: goto tr94;
		case 13: goto tr94;
		case 39: goto tr94;
		case 92: goto st19;
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 39 )
		goto tr30;
	goto tr24;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	goto st18;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st47;
	goto tr96;
tr80:
#line 1 "NONE"
	{te = p+1;}
#line 405 "klscan.rl"
	{act = 28;}
	goto st48;
tr111:
#line 1 "NONE"
	{te = p+1;}
#line 394 "klscan.rl"
	{act = 25;}
	goto st48;
tr115:
#line 1 "NONE"
	{te = p+1;}
#line 381 "klscan.rl"
	{act = 12;}
	goto st48;
tr119:
#line 1 "NONE"
	{te = p+1;}
#line 384 "klscan.rl"
	{act = 15;}
	goto st48;
tr125:
#line 1 "NONE"
	{te = p+1;}
#line 391 "klscan.rl"
	{act = 22;}
	goto st48;
tr131:
#line 1 "NONE"
	{te = p+1;}
#line 387 "klscan.rl"
	{act = 18;}
	goto st48;
tr137:
#line 1 "NONE"
	{te = p+1;}
#line 390 "klscan.rl"
	{act = 21;}
	goto st48;
tr138:
#line 1 "NONE"
	{te = p+1;}
#line 392 "klscan.rl"
	{act = 23;}
	goto st48;
tr145:
#line 1 "NONE"
	{te = p+1;}
#line 395 "klscan.rl"
	{act = 26;}
	goto st48;
tr151:
#line 1 "NONE"
	{te = p+1;}
#line 386 "klscan.rl"
	{act = 17;}
	goto st48;
tr153:
#line 1 "NONE"
	{te = p+1;}
#line 382 "klscan.rl"
	{act = 13;}
	goto st48;
tr159:
#line 1 "NONE"
	{te = p+1;}
#line 385 "klscan.rl"
	{act = 16;}
	goto st48;
tr161:
#line 1 "NONE"
	{te = p+1;}
#line 389 "klscan.rl"
	{act = 20;}
	goto st48;
tr165:
#line 1 "NONE"
	{te = p+1;}
#line 383 "klscan.rl"
	{act = 14;}
	goto st48;
tr167:
#line 1 "NONE"
	{te = p+1;}
#line 388 "klscan.rl"
	{act = 19;}
	goto st48;
tr171:
#line 1 "NONE"
	{te = p+1;}
#line 393 "klscan.rl"
	{act = 24;}
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 1457 "klscan.cpp"
	if ( (*p) == 95 )
		goto tr80;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr97;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 95: goto tr80;
		case 99: goto st50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 95: goto tr80;
		case 99: goto st51;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto st52;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto st54;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 9: goto st55;
		case 10: goto tr106;
		case 13: goto st55;
		case 32: goto st55;
		case 95: goto tr80;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr104;
tr106:
#line 328 "klscan.rl"
	{ curline++; }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 1582 "klscan.cpp"
	switch( (*p) ) {
		case 9: goto st55;
		case 10: goto tr106;
		case 13: goto st55;
		case 32: goto st55;
	}
	goto tr104;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 95: goto tr80;
		case 108: goto st57;
		case 111: goto st60;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 95: goto tr80;
		case 97: goto st58;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto st59;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto tr111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 95: goto tr80;
		case 109: goto st61;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 95: goto tr80;
		case 109: goto st62;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 95: goto tr80;
		case 105: goto st63;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 95: goto tr80;
		case 116: goto tr115;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 95: goto tr80;
		case 105: goto st65;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 95: goto tr80;
		case 110: goto st66;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 95: goto tr80;
		case 97: goto st67;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 95: goto tr80;
		case 108: goto tr119;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 95: goto tr80;
		case 110: goto st69;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 95: goto tr80;
		case 99: goto st70;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 95: goto tr80;
		case 108: goto st71;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 95: goto tr80;
		case 117: goto st72;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 95: goto tr80;
		case 100: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto tr125;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 95: goto tr80;
		case 111: goto st75;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 95: goto tr80;
		case 110: goto st76;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 95: goto tr80;
		case 116: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto st78;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 95: goto tr80;
		case 114: goto st79;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 95: goto tr80;
		case 109: goto tr131;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 95: goto tr80;
		case 97: goto st81;
		case 114: goto st85;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 95: goto tr80;
		case 114: goto st82;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto st83;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto st84;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 95: goto tr80;
		case 114: goto tr137;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 95: goto tr80;
		case 105: goto tr138;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 95: goto tr80;
		case 104: goto st87;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 95: goto tr80;
		case 111: goto st88;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 95: goto tr80;
		case 114: goto st89;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 95: goto tr80;
		case 116: goto st90;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto st91;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto st92;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 95: goto tr80;
		case 116: goto tr145;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 95: goto tr80;
		case 111: goto st94;
		case 114: goto st97;
		case 121: goto st104;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 95: goto tr80;
		case 107: goto st95;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto st96;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 95: goto tr80;
		case 110: goto tr151;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 95: goto tr80;
		case 97: goto st98;
		case 121: goto tr153;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 95: goto tr80;
		case 110: goto st99;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 95: goto tr80;
		case 108: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 95: goto tr80;
		case 97: goto st102;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 95: goto tr80;
		case 116: goto st103;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto tr159;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 95: goto tr80;
		case 112: goto st105;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto tr161;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 95: goto tr80;
		case 110: goto st107;
		case 115: goto st109;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 95: goto tr80;
		case 100: goto st108;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 95: goto tr80;
		case 111: goto tr165;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto st110;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	switch( (*p) ) {
		case 95: goto tr80;
		case 115: goto tr167;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 95: goto tr80;
		case 114: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 95: goto tr80;
		case 105: goto st113;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 95: goto tr80;
		case 116: goto st114;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 95: goto tr80;
		case 101: goto tr171;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else
		goto tr80;
	goto tr98;
tr92:
#line 1 "NONE"
	{te = p+1;}
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
#line 2607 "klscan.cpp"
	if ( (*p) == 37 )
		goto st20;
	goto tr94;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 37 )
		goto tr32;
	goto tr24;
	}
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 22: goto tr43;
	case 23: goto tr44;
	case 1: goto tr0;
	case 2: goto tr0;
	case 24: goto tr44;
	case 25: goto tr46;
	case 26: goto tr44;
	case 3: goto tr0;
	case 4: goto tr0;
	case 27: goto tr44;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 28: goto tr48;
	case 29: goto tr49;
	case 31: goto tr62;
	case 32: goto tr63;
	case 8: goto tr12;
	case 9: goto tr12;
	case 33: goto tr63;
	case 34: goto tr66;
	case 35: goto tr63;
	case 10: goto tr12;
	case 11: goto tr12;
	case 36: goto tr63;
	case 12: goto tr12;
	case 13: goto tr12;
	case 14: goto tr12;
	case 37: goto tr67;
	case 38: goto tr63;
	case 39: goto tr68;
	case 41: goto tr72;
	case 43: goto tr93;
	case 44: goto tr94;
	case 15: goto tr24;
	case 16: goto tr24;
	case 45: goto tr94;
	case 17: goto tr24;
	case 46: goto tr94;
	case 18: goto tr24;
	case 19: goto tr24;
	case 47: goto tr96;
	case 48: goto tr97;
	case 49: goto tr98;
	case 50: goto tr98;
	case 51: goto tr98;
	case 52: goto tr98;
	case 53: goto tr98;
	case 54: goto tr104;
	case 55: goto tr104;
	case 56: goto tr98;
	case 57: goto tr98;
	case 58: goto tr98;
	case 59: goto tr98;
	case 60: goto tr98;
	case 61: goto tr98;
	case 62: goto tr98;
	case 63: goto tr98;
	case 64: goto tr98;
	case 65: goto tr98;
	case 66: goto tr98;
	case 67: goto tr98;
	case 68: goto tr98;
	case 69: goto tr98;
	case 70: goto tr98;
	case 71: goto tr98;
	case 72: goto tr98;
	case 73: goto tr98;
	case 74: goto tr98;
	case 75: goto tr98;
	case 76: goto tr98;
	case 77: goto tr98;
	case 78: goto tr98;
	case 79: goto tr98;
	case 80: goto tr98;
	case 81: goto tr98;
	case 82: goto tr98;
	case 83: goto tr98;
	case 84: goto tr98;
	case 85: goto tr98;
	case 86: goto tr98;
	case 87: goto tr98;
	case 88: goto tr98;
	case 89: goto tr98;
	case 90: goto tr98;
	case 91: goto tr98;
	case 92: goto tr98;
	case 93: goto tr98;
	case 94: goto tr98;
	case 95: goto tr98;
	case 96: goto tr98;
	case 97: goto tr98;
	case 98: goto tr98;
	case 99: goto tr98;
	case 100: goto tr98;
	case 101: goto tr98;
	case 102: goto tr98;
	case 103: goto tr98;
	case 104: goto tr98;
	case 105: goto tr98;
	case 106: goto tr98;
	case 107: goto tr98;
	case 108: goto tr98;
	case 109: goto tr98;
	case 110: goto tr98;
	case 111: goto tr98;
	case 112: goto tr98;
	case 113: goto tr98;
	case 114: goto tr98;
	case 115: goto tr94;
	case 20: goto tr24;
	}
	}

	_out: {}
	}

#line 521 "klscan.rl"

		/* Check if we failed. */
		if ( cs == klscan_error ) {
			/* Machine failed before finding a token. */
			cout << "PARSE ERROR" << endl;
			exit(1);
		}

		/* Decide if we need to preserve anything. */
		char *preserve = ts;
		if ( inline_start != 0 && ( preserve == 0 || inline_start < preserve ) )
			preserve = inline_start;

		/* Now set up the prefix. */
		if ( preserve == 0 )
			have = 0;
		else {
			/* There is data that needs to be shifted over. */
			have = pe - preserve;
			memmove( buf, preserve, have );
			unsigned int shiftback = preserve - buf;
			if ( ts != 0 )
				ts -= shiftback;
			if ( inline_start != 0 )
				inline_start -= shiftback;

			te -= shiftback;
			litstart -= shiftback;
			litend -= shiftback;
			identstart -= shiftback;
			identend -= shiftback;

			preserve = buf;
		}
	}

	delete[] buf;
}

void scan( const char *fileName, istream &input )
{
	ParserDict parserDict;
	Scanner scanner( fileName, input, parserDict, 0 );
	scanner.do_scan();

	/* This uses the most recent parser definition. */
	if ( ::generateGraphviz && ::graphvizGenParser != 0 )
		graphvizGenParser->pd.generateGraphviz();
}
