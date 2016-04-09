# New Makefile that automatically depends itself
#
# $Id: Makefile,v 1.3 1996/12/17 19:52:37 chase Exp $
#

IFLAGS = -I/usr/local/include
DFLAGS = 
CXX = g++ --std=c++11
CC  = $(GCC)
GCC = g++ --std=c++11
LD  = $(CXX)

# On Mac OS/X you may need the –framework Cocoa flag
LIBS = -lfltk -lX11 -ldl -pthread
# LIBS = -lfltk -lX11 -ldl –pthread –framework Cocoa

WFLAGS = -Wall 
SYMFLAGS = -g

PROFILE = #-pg 
OPTFLAGS =#-O
CFLAGS = $(OPTFLAGS) $(PROFILE) $(WFLAGS) $(IFLAGS) $(SYMFLAGS)
CXXFLAGS = $(CFLAGS)
CPPFLAGS = $(IFLAGS) $(DFLAGS)
LDFLAGS = $(PROFILE) -g -L/usr/local/lib -L/opt/X11/lib

PROGRAM = bugs
#CXXSRCS = Source.cpp

CXXSRCS = main_linux.cpp Project7-solution.cpp
          

SRCS = $(CXXSRCS) 

OBJS = $(CXXSRCS:.cpp=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

test: $(PROGRAM)
	./$(PROGRAM)

clean:
	-rm -f $(OBJS) $(PROGRAM)

tidy:
	-rm -f *.BAK *.bak *.CKP

undepend:
	-rm -f $(OBJS:%.o=.%.d) 

spotless: tidy clean undepend

.y.cpp:
	$(BISON) $(BISONFLAGS) -o $@ $<
	mv $@.h $*.h
	mv $@.output $*.output
.l.cpp:
	$(FLEX) ${FLEXFLAGS} -t $< > $@

# auto depend stuff for GNU make only
depend: undepend
	@echo ""
	@echo "Dependences are handled automatically, just \"make\""

ifneq ($(strip $(CSRCS)),)
.%.d: %.c 
	$(SHELL) -ec '$(GCC) -MM $(CPPFLAGS) $< > $@'


include $(CSRCS:%.c=.%.d)
endif 

ifneq ($(strip $(CXXSRCS)),)
.%.d: %.cpp
	$(SHELL) -ec '$(GCC) -MM $(CPPFLAGS) $< > $@'

include $(CXXSRCS:%.cpp=.%.d) 
endif 

