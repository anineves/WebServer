#include "../includes/Utils.hpp"
#include "../includes/webServer.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>




void printMap(const std::map<std::string, std::string>& map) 
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}


void log(const std::string &message) {
    std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage) {
    log("ERROR: " + errorMessage);
    //exit(1);
}

std::string convert_uint32_to_str(u_int32_t nb) {
    std::stringstream ss;
    ss << ((nb >> 24) & 0xFF) << '.' << ((nb >> 16) & 0xFF) << '.' << ((nb >> 8) & 0xFF) << '.' << (nb & 0xFF);

    return (ss.str().c_str());
}

void *ft_memset(void *s, int c, std::size_t n) {
    unsigned char *f;

    f = (unsigned char *)s;
    while (n--)
        *f++ = c;
    return (s);
}


int ft_stoi(std::string str) {
    std::stringstream ss(str);
    int num = 0;
    ss >> num;
    return num;
}

void printVector(const std::vector<std::string>& vec) {

    std::cout << MAGENTA << "Server Names" << RESET << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}


u_int32_t str_to_uint32(std::string str) {
    std::istringstream iss(str);
    uint32_t result = 0;
    unsigned int oct;

    for (int i = 0; i < 4; ++i) {
        char p;
        if (i > 0) {
            if (!(iss >> p) || p != '.') {
                return 0;
            }
        }
        if (!(iss >> oct || oct > 255)) {
            return 0;
        }

        result = (result << 8) | oct;
    }

    return result;
}




bool fileExists(const std::string filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}


std::string dirListHtml(std::vector<std::string> content)
{
    std::string response;
    std::string htmlCode;

    htmlCode.append("<!DOCTYPE html>\n"
                    "<html lang=\"en\">\n"
                    "<head>\n"
                    "   <meta charset=\"UTF-8\">\n"
                    "   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                    "   <title>Directory List</title>\n"
                    "   <style>\n"
                    "       html, body {\n"
                    "           font-family: Arial, sans-serif;\n"
                    "           height: 100%;\n"
                    "           margin: 0;\n"
                    "           color: #000;\n"
                    "           background-color: #007bff;\n"
                    "       }\n"
                    "\n"
                    "       .container {\n"
                    "           text-align: center;\n"
                    "           max-width: 500px;\n"
                    "           margin: 0 auto;\n"
                    "           padding: 20px;\n"
                    "           color:#ffffff;\n" 
                    "           background-color: linear-gradient(to right, rgb(20, 17, 25) 0%, rgb(69, 61, 52) 80%, rgb(57, 50, 45) 100%);\n"
                    "           border-radius: 10px;\n"
                    "       }\n"
                    "   </style>\n"
                    "</head>\n"
                    "<body>\n"
                    "   <div class=\"container\">\n"
                    "       <h1>Directory List</h1>\n"
                    "       <ul>\n");

    htmlCode.append("           <li><a href=\"/\">Back</a></li>\n");

    for (size_t i = 0; i < content.size(); ++i)
    {
        htmlCode.append("           <li>" + content[i] + "</li>\n");
    }

    htmlCode.append("       </ul>\n"
                    "   </div>\n"
                    "</body>\n"
                    "<script>\n"
                    "   function deleteFile(fileName) {\n"
                    "       if (confirm('Are you sure you want to delete ' + fileName + '?')) {\n"
                    "           fetch('/delete?fileName=' + fileName, { method: 'GET' })\n"
                    "               .then(response => {\n"
                    "                   if (!response.ok) {\n"
                    "                       throw new Error('Network response was not ok');\n"
                    "                   }\n"
                    "                   return response.text();\n"
                    "               })\n"
                    "               .then(data => {\n"
                    "                   alert(data);\n"
                    "                   location.reload();\n"
                    "               })\n"
                    "               .catch(error => {\n"
                    "                   console.error('There has been a problem with your fetch operation:', error);\n"
                    "               });\n"
                    "       }\n"
                    "   }\n"
                    "</script>\n"
                    "</html>");

    std::stringstream contentSize;
    contentSize << htmlCode.size();
    response.append("HTTP/1.1 200 OK\r\n");
    response.append("Content-Type: text/html\r\n");
    response.append("Content-Length: " + contentSize.str() + "\r\n");
    response.append("\r\n");
    response.append(htmlCode);

    return response;
}

std::string intToString(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string deleteFile(std::string fileName) {
    std::string filePath = "frontend/html/" + fileName;
    if (std::remove(filePath.c_str()) != 0) {
        return "Erro ao excluir o arquivo " + fileName + ".";
    } else {
        return "Arquivo " + fileName + " excluído com sucesso!";
    }
}

int is_file(std::string path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0)
        return 0; 

    return S_ISREG(buffer.st_mode);
}



int is_directory(const std::string& path) {
    struct stat buffer;  
    if (stat(path.c_str(), &buffer) != 0)
        return 0;  
    return S_ISDIR(buffer.st_mode);
}