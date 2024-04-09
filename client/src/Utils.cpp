#include "../includes/Utils.hpp"
#include "../includes/webServer.hpp"

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




std::string dirListHtml(std::vector<std::string> content)
{
    std::string response;
    std::string htmlCode;

    htmlCode.append("<!DOCTYPE html>\n"
                    "<html lang=\"en\">\n"
                    "<head>\n"
                    "	<meta charset=\"UTF-8\">\n"
                    "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                    "	<title>Directory Lis</title>\n"
                    "	<style>\n"
                    "		html, body {\n"
                    "			font-family: Arial, sans-serif;\n"
                    "			height: 100%;\n"
                    "			margin: 0;\n"
                    "           color: #000;\n"
                    "			background-color: #ddd;\n"
                    "		}\n"
                    "\n"
                    "		.container {\n"
                    "			text-align: center;\n"
                    "			max-width: 500px;\n"
                    "			margin: 0 auto;\n"
                    "			padding: 20px;\n"
                    "           color:#ffffff;\n" 
                    "			background-color: linear-gradient(to right, rgb(20, 17, 25) 0%, rgb(69, 61, 52) 80%, rgb(57, 50, 45) 100%);\n"
                    "			border-radius: 10px;\n"
                    "		}\n"
                    "	</style>\n"
                    "</head>\n"
                    "<body>\n"
                    "	<div class=\"container\">\n"
                    "		<h1>DirList</h1>\n"
                    "		<ul>\n");

    std::cout << RED << "content.size() = " << RESET << content.size() << std::endl;
    
    for (unsigned i = 0; i < content.size(); i++)
    {
        std::cout << RED << "content[i] = " << RESET << content[i] << std::endl;
        htmlCode.append("			<li><a href=\"" + content[i] + "\">" + content[i] + "</a></li>\n");
    }

    htmlCode.append("		</ul>\n"
                    "	</div>\n"
                    "</body>\n"
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

int isvalid(std::string fullPath) {
    std::ifstream readPath(("frontend/html/" + fullPath ).c_str());
    std::cout << "!!!!!!!!!!FullPath " << readPath << std::endl;


    struct stat buffer;
    if (stat(fullPath.c_str(), &buffer) != 0) {
        return (0); 
    }
    return S_ISREG(buffer.st_mode);

}



/*int isFile(const char* path) 
{
    struct stat buffer;
    if (stat(path, &buffer) != 0) {
        return (1); // Falha ao obter informações sobre o arquivo
    }
    return (0); // Verifica se é um arquivo regular
}*/