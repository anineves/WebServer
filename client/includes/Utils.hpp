#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

void log(const std::string &message);
void exitWithError(const std::string &errorMessage);

#endif