################################################################################
######################### User configurable parameters #########################
# filename extensions
CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

# probably shouldn't modify these, but you may need them below
ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include

WARNFLAGS+=
EXTRA_CFLAGS=
EXTRA_CXXFLAGS=

# Set to 1 to enable hot/cold linking
USE_PACKAGE:=1

# Add libraries you do not wish to include in the cold image here
# EXCLUDE_COLD_LIBRARIES:= $(FWDIR)/your_library.a
EXCLUDE_COLD_LIBRARIES:= 

# Set this to 1 to add additional rules to compile your project as a PROS library template
IS_LIBRARY:=0
# TODO: CHANGE THIS!
LIBNAME:=libbest
VERSION:=1.0.0
# EXCLUDE_SRC_FROM_LIB= $(SRCDIR)/unpublishedfile.c
# this line excludes opcontrol.c and similar files
EXCLUDE_SRC_FROM_LIB+=$(foreach file, $(SRCDIR)/main,$(foreach cext,$(CEXTS),$(file).$(cext)) $(foreach cxxext,$(CXXEXTS),$(file).$(cxxext)))

# files that get distributed to every user (beyond your source archive) - add
# whatever files you want here. This line is configured to add all header files
# that are in the the include directory get exported
TEMPLATE_FILES=$(INCDIR)/vexv5/*.h $(INCDIR)/vexv5/*.hpp

left_side_winpoint: quick .PHONY
	pros upload --slot 1 --name "Left side winpoint"
right_side_winpoint: quick .PHONY
	pros upload --slot 2 --name "Right side winpoint"
middle_left_goal: quick .PHONY
	pros upload --slot 3 --name "Middle left goal"
middle_right_goal: quick .PHONY
	pros upload --slot 4 --name "Middle right goal"
reset: quick .PHONY
	pros upload --slot 6 --name "Reset"
basic: quick .PHONY
	pros upload --slot 5 --name "Basic"

left_side_winpoint: EXTRA_CFLAGS = -D LEFT_SIDE_WINPOINT
left_side_winpoint: EXTRA_CXXFLAGS = -D LEFT_SIDE_WINPOINT

right_side_winpoint: EXTRA_CFLAGS = -D RIGHT_SIDE_WINPOINT
right_side_winpoint: EXTRA_CXXFLAGS = -D RIGHT_SIDE_WINPOINT

middle_left_goal: EXTRA_CFLAGS = -D MIDDLE_LEFT_GOAL
middle_left_goal: EXTRA_CXXFLAGS = -D MIDDLE_LEFT_GOAL

middle_right_goal: EXTRA_CFLAGS = -D MIDDLE_RIGHT_GOAL
middle_right_goal: EXTRA_CXXFLAGS = -D MIDDLE_RIGHT_GOAL

reset: EXTRA_CFLAGS = -D RESET_POSITIONS
reset: EXTRA_CXXFLAGS = -D RESET_POSITIONS

.DEFAULT_GOAL=quick

################################################################################
################################################################################
########## Nothing below this line should be edited by typical users ###########
-include ./common.mk
