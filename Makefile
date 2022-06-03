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
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D RESET_POSITIONS" EXTRA_CXXFLAGS="-D RESET_POSITIONS" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 1 --after none --name "Reset"

replay: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D REPLAY_MATCH" EXTRA_CXXFLAGS="-D REPLAY_MATCH" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 2 --after none --name "Replay"

left_side_winpoint: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D LEFT_SIDE_WINPOINT" EXTRA_CXXFLAGS="-D LEFT_SIDE_WINPOINT" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 3 --after none --name "Left Winpoint"

right_side_winpoint: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D RIGHT_SIDE_WINPOINT" EXTRA_CXXFLAGS="-D RIGHT_SIDE_WINPOINT" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 4 --after none --name "Right Winpoint"

middle_left_goal: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D MIDDLE_LEFT_GOAL" EXTRA_CXXFLAGS="-D MIDDLE_LEFT_GOAL" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 5 --after none --name "Left Goal"

middle_right_goal: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D MIDDLE_RIGHT_GOAL" EXTRA_CXXFLAGS="-D MIDDLE_RIGHT_GOAL" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 6 --after none --name "Right Goal"

record: 
	$(MAKE) $(MAKEFLAGS) quick EXTRA_CFLAGS="-D RECORD_MATCH" EXTRA_CXXFLAGS="-D RECORD_MATCH" -W src/switches.cpp -W include/vexv5/switches.hpp
	pros upload --slot 7 --after none --name "Record"

.DEFAULT_GOAL=quick

################################################################################
################################################################################
########## Nothing below this line should be edited by typical users ###########
-include ./common.mk
