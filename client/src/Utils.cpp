#include "../includes/Utils.hpp"
#include "../includes/webServer.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

#include <fstream>
#include <experimental/filesystem>



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




std::string handleRequest(std::string request)
{
    // Analisar a solicitação
    std::istringstream iss(request);
    std::string method, uri, protocol;
    iss >> method >> uri >> protocol;

    if (method == "GET" && uri.find("/delete") == 0) {
        std::string fileName;
        size_t pos = uri.find('=');
        if (pos != std::string::npos) {
            fileName = uri.substr(pos + 1);
            return deleteFile(fileName);
        }
    }

    // Se a solicitação não for reconhecida ou não puder ser manipulada, retorne um erro
    return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request.";
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
                    "           background-color: #ddd;\n"
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
                    "       .delete-button {\n"
                    "           background-color: #ff4d4d;\n"
                    "           border: none;\n"
                    "           color: white;\n"
                    "           padding: 8px 20px;\n"
                    "           text-align: center;\n"
                    "           text-decoration: none;\n"
                    "           display: inline-block;\n"
                    "           font-size: 14px;\n"
                    "           margin: 4px 2px;\n"
                    "           cursor: pointer;\n"
                    "           border-radius: 5px;\n"
                    "       }\n"
                    "       .delete-button:hover {\n"
                    "           background-color: #ff6666;\n"
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
        htmlCode.append("           <li>" + content[i] + " <button id=\"delete-button-" + intToString(i) + "\" class=\"delete-button\" onclick=\"deleteFile('" + content[i] + "')\">X</button></li>\n");
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