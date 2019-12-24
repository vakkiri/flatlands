/*
 * logging.cpp
 *
 */

#include <iostream>

#include "logging.h"

const std::string progress_start = ">> ";
const std::string progress_end = "...\n";
const std::string error_start = "\nXX ";
const std::string error_end = "\n\n";
const std::string warning_start = "!! ";
const std::string warning_end = "\n";

void log_progress(std::string str) {
	std::cout << progress_start << str << progress_end;
}

void log_error(std::string str) {
	std::cout << error_start << str << error_end;
}

void log_warning(std::string str) {
	std::cout << warning_start << str << warning_end;
}

