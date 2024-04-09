#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <cstring>

void log(const std::string &message);
void exitWithError(const std::string &errorMessage);
std::string convert_uint32_to_str(u_int32_t nb);
void *ft_memset(void *s, int c, std::size_t n);
std::string dirListHtml(std::vector<std::string> content);
int isvalid(std::string fullPath);
int isFile(const char* path);
#endif