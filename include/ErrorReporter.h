#pragma once

#include "IncludeLibs.h"

#define REPORT_ERROR(error, func) printError(error, #func, __FILE__, __LINE__);

inline void printError(const std::string& error, const char* functionName, const char* file, const int lineNumber) {
	printf(
		"[ERROR] - %s\n"
		"[FUNCTION] - %s\n"
		"[FILE] - %s\n"
		"[LINE] - %d\n\n",
		error.c_str(), functionName, file, lineNumber
	);
}