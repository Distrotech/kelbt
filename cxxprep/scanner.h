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

#ifndef _SCANNER_H
#define _SCANNER_H

#include <iostream>
#include "bstset.h"
#include "parser.h"

extern BstSet<char*, CmpStr> ignoreKeys;

struct Scanner
{
	Scanner( std::ostream &out, char *inputFile )
		: out(out), line(0), parser(out)
	{
	}

	std::ostream &out;

	int line;
	Parser parser;
	StrAlloc strAlloc;
	Buffer leaderBuf;

	void token( int tok, bool qual = false );
	void finish();

	int cs, act;
	char *tokstart, *tokend;

	void init();
	int execute( char *, int len );
};

#endif /* _SCANNER_H */
