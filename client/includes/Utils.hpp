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
int ft_stoi(std::string str);

void log(const std::string &message);
void exitWithError(const std::string &errorMessage);
std::string convert_uint32_to_str(u_int32_t nb);
void *ft_memset(void *s, int c, std::size_t n);
std::string dirListHtml(std::vector<std::string> content);
int isvalid(std::string fullPath);
int is_file(std::string path);
std::string deleteFile(std::string fileName);
std::string intToString(int num);
std::string verificDelete(std::vector<std::string> content);
std::string handleRequest(std::string request);
u_int32_t str_to_uint32(std::string str);
void printVector(const std::vector<std::string>& vec);
std::string consumeSpaces(std::string line);
int restSpaces(std::string line, int pos);
std::string consumeMultiSpaces(std::string line);
int consumeComments(std::string line);
int consumeEmptyLines(std::string line);
std::vector<std::string>    putKeys(std::vector<std::string> keyVec);
std::vector<std::string> getkeys(std::vector<std::string> fileVec);
std::vector<std::string> removeBrackets(std::vector<std::string> fileVec);
int checkkey(std::string line, std::vector<std::string> vector);
void    matchKeys(std::vector<std::string> keyVec, std::vector<std::string> fileVec);
void    matchingServerLocsBrackets(std::vector<std::string> tmp, std::vector<std::string> fileVec);
void    verifyBlocks(std::vector<std::string> fileVec);
void    checkServerLocs(std::vector<std::string> fileVec);
void    listenRule(std::string line);
void    serverNameRule(std::string line);
void    hostRule(std::string line);
void    verifyVar(std::vector<std::string> fileVec);
void    verifyValues(std::vector<std::string> fileVec);


#endif