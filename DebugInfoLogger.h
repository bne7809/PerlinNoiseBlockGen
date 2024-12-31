#pragma once

#include <string>

#define LOG_STR(s)				OutputDebugStringA((std::string(s) + "\n").c_str())
#define LOG_VAR_INFO(s, v)		OutputDebugStringA((s + std::to_string(v) + "\n").c_str())