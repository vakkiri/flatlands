/*
 * 	logging.h
 *
 * 	Utility methods for printing logs to the console.
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <string>

void log_progress(std::string str);
void log_warning(std::string str);
void log_error(std::string str);

#endif
