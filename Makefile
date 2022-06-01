################################################################################
######################### User configurable parameters #########################
# filename extensions
CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include

WARNFLAGS+=
EXTRA_CFLAGS=
EXTRA_CXXFLAGS=

USE_PACKAGE:=1
EXCLUDE_COLD_LIBRARIES:= 

IS_LIBRARY:=0
LIBNAME:=vexv5
VERSION:=1.0.0
EXCLUDE_SRC_FROM_LIB+=$(foreach file, $(SRCDIR)/main,$(foreach cext,$(CEXTS),$(file).$(cext)) $(foreach cxxext,$(CXXEXTS),$(file).$(cxxext)))
TEMPLATE_FILES=$(INCDIR)/vexv5/*.h $(INCDIR)/vexv5/*.hpp

reset: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D RESET_POSITIONS" EXTRA_CXXFLAGS="-O3 -D RESET_POSITIONS" -W src/switches.cpp
	pros upload --slot 1 --icon X --after none

replay_match: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D REPLAY_MATCH" EXTRA_CXXFLAGS="-O3 -D REPLAY_MATCH" -W src/switches.cpp
	pros upload --slot 2 --icon ufo --after none

left_side_winpoint: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D LEFT_SIDE_WINPOINT" EXTRA_CXXFLAGS="-O3 -D LEFT_SIDE_WINPOINT" -W src/switches.cpp
	pros upload --slot 3 --icon robot --after none

right_side_winpoint: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D RIGHT_SIDE_WINPOINT" EXTRA_CXXFLAGS="-O3 -D RIGHT_SIDE_WINPOINT" -W src/switches.cpp
	pros upload --slot 4 --icon clawbot --after none

middle_left_goal: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D MIDDLE_LEFT_GOAL" EXTRA_CXXFLAGS="-O3 -D MIDDLE_LEFT_GOAL" -W src/switches.cpp
	pros upload --slot 5 --icon pizza --after none

middle_right_goal: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D MIDDLE_RIGHT_GOAL" EXTRA_CXXFLAGS="-O3 -D MIDDLE_RIGHT_GOAL" -W src/switches.cpp
	pros upload --slot 6 --icon planet --after none

record_match: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-O3 -D RECORD_MATCH" EXTRA_CXXFLAGS="-O3 -D RECORD_MATCH" -W src/switches.cpp
	pros upload --slot 7 --icon question --after none

.DEFAULT_GOAL=quick

################################################################################
################################################################################
########## Nothing below this line should be edited by typical users ###########
-include ./common.mk
