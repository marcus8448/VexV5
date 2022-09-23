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
	$(MAKE) -$(MAKEFLAGS) quick EXTRA_CFLAGS="-D RESET_POSITIONS" EXTRA_CXXFLAGS="-D RESET_POSITIONS" -W src/main.cpp
	pros upload --slot 1 --after none --name "Reset"

replay:
	$(MAKE) -$(MAKEFLAGS) quick EXTRA_CFLAGS="-D REPLAY_MATCH" EXTRA_CXXFLAGS="-D REPLAY_MATCH" -W src/main.cpp
	pros upload --slot 2 --after none --name "Replay"

record:
	$(MAKE) -$(MAKEFLAGS) quick EXTRA_CFLAGS="-D RECORD_MATCH" EXTRA_CXXFLAGS="-D RECORD_MATCH" -W src/main.cpp
	pros upload --slot 3 --after none --name "Record"

normal:
	$(MAKE) -$(MAKEFLAGS) quick -W src/main.cpp
	pros upload --slot 8 --after none --name "Normal"

buildonly:
	$(MAKE) -$(MAKEFLAGS) quick -W src/main.cpp

.DEFAULT_GOAL=buildonly

################################################################################
################################################################################
########## Nothing below this line should be edited by typical users ###########
-include ./common.mk
