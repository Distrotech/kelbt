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

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "names.h"

using namespace std;

char *inputName = 0;
char *targetName = 0;
char *outputName = 0;
ostream *outputFile;

/* EOF char used to flush out that last token. This should be a whitespace
 * token. */
#define LAST_CHAR 0

void feedToScanner( FILE *input )
{
	#define BUFSIZE 16384
	static char buf[BUFSIZE];

	/* Create and initialize the scanner. */
	Scanner scanner( *outputFile, inputName );
	scanner.init();

	bool done = false;
	int have = 0;
	while ( !done ) {
		char *data = buf + have;
		int space = BUFSIZE - have;

		if ( space == 0 ) {
			/* We filled up the buffer trying to scan a token. */
			cerr << "OUT OF BUFFER SPACE" << endl;
			exit(1);
		}

		int len = fread( data, 1, space, input );
		if ( len == 0 ) {
			data[0] = LAST_CHAR, len++;
			done = true;
		}

		int rtn = scanner.execute( data, len );
		if ( rtn < 0 ) {
			cerr << "SCANNER ERROR" << endl;
			exit(1);
		}

		/* Now set up the prefix. */
		if ( scanner.tokstart == 0 )
			have = 0;
		else {
			/* There is data that needs to be shifted over. */
			have = data + len - scanner.tokstart;
			memmove( buf, scanner.tokstart, have );
			scanner.tokend -= (scanner.tokstart-buf);
			scanner.tokstart = buf;
		}
	}
	scanner.finish();
}

void usage()
{
	cout << 
		"cxxprep [options] file\n"
		"    preprocess and parse the input file, uses g++ for preprocessing\n"
		"\n"
		"cxxprep [options] -n [file]\n"
		"    inhibit the preprocessor, only parse the input file (or standard in)\n"
		"\n"
		"cxxprep [options] -p file\n"
		"    inhibit the parser, only run the preprocessor\n"
		"\n"
		"options:\n"
		"    -h -H -?         print usage and exit\n"
		"    -I dir           include directories to pass to the preprocessor\n"
		"    -D macro_def     macro defintiions to pass to the preprocessor\n"
		"    -k keyword       keywords for the parser to ignore\n";
}

int main( int argc, char **argv )
{
	std::ios::sync_with_stdio(false);
	while ( true ) {
		int opt = getopt( argc, argv, "hH?t:o:" );
		if ( opt < 0 )
			break;

		switch ( opt ) {
		case 't':
			targetName = ::optarg;
			break;
		case 'o':
			outputName = ::optarg;
			break;
		case 'h': case 'H': case '?':
			usage();
			return 0;
		}
	}

	/* Open the input file. */
	if ( optind < argc ) {
		inputName = argv[optind];
		std::ifstream in( inputName );
		if ( ! in.is_open() ) {
			cerr << "cxxprep: failed to open input file " << inputName << endl;
			exit(1);
		}
	}

	outputFile = &cout;
	if ( outputName != 0 ) {
		std::ofstream *out = new std::ofstream( outputName );
		if ( ! out->is_open() ) {
			cerr << "cxxprep: failed to open output file " << outputName << endl;
			exit(1);
		}
		outputFile = out;
	}

	if ( targetName == 0 )
		targetName = inputName;

	FILE *input = inputName != 0 ? fopen( inputName, "rt" ) : stdin;
	feedToScanner( input );

	return 0;
}
