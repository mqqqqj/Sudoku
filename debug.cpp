#include "function.h"
#include <iostream>

void Log(const char* buf, int type) {
	switch (type) {
	case 1:std::cout << "[LOG]:"; break;
	case 2:std::cout << "[WARN]:"; break;
	case 3:std::cout << "[ERROR]:"; break;
	default:return;
	}
	std::cout << buf << std::endl;
}
