#ifndef _MISC_DEFINITIONS_H_
#define _MISC_DEFINITIONS_H_

#include <string>

enum ControlState {
	INITIALIZE,
	COMPETITION_INITIALIZE,
	AUTONOMOUS,
	OPERATOR_CONTROL,
	DISABLED
};

std::string stateName(ControlState state) {
	switch (state) {
		case INITIALIZE:
			return "Initialize";
		case COMPETITION_INITIALIZE:
			return "Competition Initialize";
		case AUTONOMOUS:
			return "Autonomous";
		case OPERATOR_CONTROL:
			return "Operator Control";
		case DISABLED:
			return "Disabled";
	}
	return "INVALID";
}

#endif // _MISC_DEFINITIONS_H_
