#
#  Copyright 2004-2006 Adrian Thurston <thurston@complang.org>
#

#  This file is part of C++ Pre-Parser.
# 
#  C++ Pre-Parser is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
# 
#  C++ Pre-Parser is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with C++ Pre-Parser; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 

TARG = scanner

INCS = -I ../aapl
DEFS =
CFLAGS = -g -Wall
LDFLAGS = -g

CC_SRCS = main.cpp scanner.cpp parser.cpp keys.cpp names.cpp
CC_HEADERS = parser.h

#*************************************

# Programs
CXX = c++
AR = ar
KELBT = ../kelbt/kelbt
RAGEL = ragel
RLCODEGEN = rlgen-cd

# Get objects, dependencies from sources.
OBJS = $(CC_SRCS:%.cpp=%.o)
DEPS = $(CC_SRCS:%.cpp=.%.d)

# Rules.
all: cxxprep undo

cxxprep: $(KELBT) $(CC_HEADERS) $(CC_SRCS) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

undo: undo.lex
	flex -oundo.c undo.lex
	gcc -o undo undo.c

keys.cpp: keys.gperf
	gperf -L C++ -t $< > $@

parser.h: parser.kh $(KELBT)
	$(KELBT) -o $@ $<
parser.cpp: parser.kl parser.kh $(KELBT)
	$(KELBT) -o $@ $<


%.cpp: %.rl
	$(RAGEL) $< | $(RLCODEGEN) -G2 -o $@

%.o: %.cpp
	@$(CXX) -M $(CFLAGS) $(DEFS) $(INCS) $< > .$*.d
	$(CXX) -c $(CFLAGS) $(DEFS) $(INCS) -o $@ $<

distclean: clean

clean:
	rm -Rf .*.d *.o cxxprep scanner.cpp \
		parser.{h,cpp} keys.cpp undo.c undo

-include $(DEPS)

PREFIX = /usr/local

install:
	install -d $(PREFIX)/bin
	install -m 755 cxxprep $(PREFIX)/bin/
