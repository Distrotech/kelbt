#
#   Copyright 2001-2003, 2005 Adrian Thurston <thurston@complang.org>
#

#   This file is part of Kelbt
#
#   Kelbt is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   Kelbt is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Kelbt; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 

INCS = -I../aapl
DEFS =

CFLAGS = -g -Wall
LDFLAGS =

CC_SRCS = \
	pcheck.cpp main.cpp parsedata.cpp closure.cpp fsmbase.cpp fsmattach.cpp \
	fsmgraph.cpp codegen.cpp gvdotgen.cpp klscan.cpp klparse.cpp


LIBS = 
prefix = /usr/local

#*************************************

# Programs
CXX = g++

# Get objects and dependencies from sources.
OBJS = $(CC_SRCS:%.cpp=%.o)
DEPS = $(CC_SRCS:%.cpp=.%.d)

# Get the version info.
include ../version.mk

# Rules.
all: kelbt

kelbt: $(GEN_SRC) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)


%.o: %.cpp
	@$(CXX) -M $(DEFS) $(INCS) $< > .$*.d
	$(CXX) -c $(CFLAGS) $(DEFS) $(INCS) -o $@ $<

install: all
	install -d $(prefix)/bin
	install -s kelbt $(prefix)/bin/kelbt

distclean: clean
	rm -f Makefile

clean:
	rm -Rf kelbt tags .*.d *.o klscan.cpp klparse.h klparse.cpp version.h

-include $(DEPS)
