#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

void log(const std::string &message);
void exitWithError(const std::string &errorMessage);
std::string convert_uint32_to_str(u_int32_t nb);


#endif