#pragma once
#include "pch.h"

class Utility {
public:
	static void StreamPrint(std::ostream& out, const char* str, ...);
	static std::string PrintFloat(float f);
};