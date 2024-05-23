#include "../includes/cParser.hpp"
cParser::cParser() {}

cParser::~cParser() {
    this->fileVec.clear();
}

std::string consumeSpaces(std::string line) {
    std::string result;
    int i = 0;
    if (line[i] == ' ') {
        while (line[i] == ' ')
            i++;
        result = line.substr(i, line.length());
    } else {
        return line;
    }
    return result;
}

int restSpaces(std::string line, int pos) {
    for (size_t i = pos; i < line.length(); i++) {
        if (line[i] != ' ')
            return 0;
    }
    return 1;
}

std::string consumeMultiSpaces(std::string line) {
    int flag = 0;
    size_t i = 0;
    std::string result;
    while (i < line.length()) {
        if (line[i] != ' ' && flag)
            flag = 0;
        if (line[i] != ' ' && !flag)
            result += line[i];
        if (line[i] == ' ' && !flag) {
            if (restSpaces(line, i))
                return result;
            result += line[i];
            flag = 1;
        }
        i++;
    }
    return result;
}

int consumeComments(std::string line) {
    if (line[0] == '#')
        return 1;
    return 0;
}

int consumeEmptyLines(std::string line) {
    if (!line.compare(""))
        return 1;
    return 0;
}

std::vector<std::string>    putKeys(std::vector<std::string> keyVec) {
    keyVec.push_back("Server");
    keyVec.push_back("Server{");
    keyVec.push_back("location");
    keyVec.push_back("listen");
    keyVec.push_back("host");
    keyVec.push_back("client_max_body_size");
    keyVec.push_back("root");
    keyVec.push_back("index");
    keyVec.push_back("autoindex");
    keyVec.push_back("server_name");
    keyVec.push_back("error_page");
    keyVec.push_back("allow_methods");
    keyVec.push_back("cgi_ext");
    keyVec.push_back("cgi_path");
    keyVec.push_back("upload_to");
    keyVec.push_back("return");
    keyVec.push_back("{");
    keyVec.push_back("}");
    return keyVec;
}

std::vector<std::string> getkeys(std::vector<std::string> fileVec) {
    std::vector<std::string> result;
    std::string line;

    for (size_t i = 0; i < fileVec.size(); i++) {
        line = fileVec[i].substr(0, fileVec[i].find_first_of(' '));
        if (line == "Server" || line == "location" || line == "}")
            result.push_back(fileVec[i]);
        else
            result.push_back(line);
    }

    return result;
}

//Temos um vector só com as keys sem brackets
std::vector<std::string> removeBrackets(std::vector<std::string> fileVec) {
    std::vector<std::string> result;
    std::string line;
    for(size_t i = 0; i < fileVec.size(); i++) {
        line = fileVec[i].substr(0, fileVec[i].find_first_of(' '));
        if (line != "}")
            result.push_back(line);
    }
    return result;
}

int checkkey(std::string line, std::vector<std::string> vector) {
    for (size_t i = 0; i < vector.size(); i++) {
        //std::cout << line << "  ==  " << vector[i] << std::endl;
        if (!line.compare(vector[i])) {
            return 1;
        }
    }
    return 0;
}

//Comparamos as key que temos do nosso ficheiro e se sao keys validas, primeira palavra de cada linha
void    matchKeys(std::vector<std::string> keyVec, std::vector<std::string> fileVec) {
    std::vector<std::string> n_fileVec;

    n_fileVec = removeBrackets(fileVec);
    for (size_t i = 0; i < n_fileVec.size(); i++) {
        if (!checkkey(n_fileVec[i], keyVec)) {
                std::cout  << RED << "[ " << n_fileVec[i] << " ] " << RESET;
                throw ("This keyword is not valid.\n\n" BOLD "Valid keywords:\n\n" RESET GREEN
                "[Server]\n[location]\n[listen]\n[host]\n[client_max_body_size]\n[root]\n[index]\n"
                "[autoindex]\n[server_name]\n[error_page]\n[allow_methods]\n[cgi_ext]\n[cgi_path]\n"
                "[upload_to]\n[return]" RESET);
        }
    }
}

void    matchingServerLocsBrackets(std::vector<std::string> tmp, std::vector<std::string> fileVec) {
    int openBrack = 0;
    int closeBrack = 0;
    int servercount = 0;
    int locationscount = 0;

    for (size_t i = 0; i < fileVec.size(); i++) {
        if (!tmp[i].find("Server")) {
            servercount++;
        }
        if (!tmp[i].find("location")) {
            locationscount++;
        }
    }
    for (size_t j = 0; j < fileVec.size(); j++) {
        for (size_t m = 0; m < fileVec[j].length(); m++) {
            if (fileVec[j][m] == '{')
                openBrack++;
            if (fileVec[j][m] == '}')
                closeBrack++;
        }
    }
    //std::cout << "openBrack = " << openBrack << std::endl;
    //std::cout << "closeBrack = " << closeBrack << std::endl;
    //std::cout << "Server = " << servercount << std::endl;
    //std::cout << "locations = " << locationscount << std::endl;
    if ((servercount + locationscount != openBrack) || (servercount + locationscount != closeBrack))
        throw ("To many or Missing Open/Close Brackets in the Configuration file");
}
/* void    verifyBrackets(std::vector) {

} */

void    verifyBlocks(std::vector<std::string> fileVec) {
    std::vector<std::string> tmp = getkeys(fileVec);
    int sBlockStart = 0;
    //int l_begin = 0;
    int s_location = 0;
    int locationStart = 0;
    
    size_t i = 0;
/*     for (size_t i = 0; i < tmp.size(); i++) {
        std::cout << i << " = " << tmp[i] << "$" << std::endl;
    } */
    //veryfiBrackets(fileVec);
    matchingServerLocsBrackets(tmp, fileVec);
    while (i < tmp.size()) {
        //std::cout << i << " = " << tmp[i]  << "$" << std::endl;
        if (!sBlockStart && !tmp[i].find("Server") 
            && (tmp[i][tmp[i].length() - 1] == '{' || tmp[i + 1] == "{")) {
            sBlockStart = 1;
            if ((tmp[i][tmp[i].length() - 1] == 'r' && tmp[i + 1] == "{")) {
                i++;
            }
        }
        else if (sBlockStart && !tmp[i].find("Server")) {
            throw ("Server Block inside a Server Block");
        }
        else if (sBlockStart && s_location && !tmp[i].find("location")) {
            throw ("location Block inside a location Block");
        }
        else if (!sBlockStart && !tmp[i].find("Server")) {
            throw ("locations or variables outside of Server Block");
        }
        else if (sBlockStart && s_location && tmp[i] == "}") {
            s_location = 0;
        }
        else if (sBlockStart && !s_location && tmp[i] == "}") {
            sBlockStart = 0;
            locationStart = 0;
        }
        else if (sBlockStart && !s_location && tmp[i].find("location") && locationStart) {
            //std::cout << i << std::endl;
            throw ("Variables of Server must be at the top, or inside of a location");
        }
        else if (!sBlockStart && !s_location && tmp[i].find("Server")) {
            throw ("You must place varuables or blocks of locations inside Server");
        }
        else if (sBlockStart && !s_location && !tmp[i].find("location") 
            && (tmp[i][tmp[i].length() - 1] == '{' || tmp[i + 1] == "{")) {
            s_location = 1;
            locationStart = 1;
        }
        i++;
    }
}

void    checkServerLocs(std::vector<std::string> fileVec) {
    for (size_t i = 0; i < fileVec.size(); i++) {
        if (!fileVec[i].find("Server") || !fileVec[i].find("location") 
            || !fileVec[i].find("{") || !fileVec[i].find("}")) {
            if (!fileVec[i].find("location")) {
                //std::cout << RED << "location found! at index: " << i << RESET << std::endl;
                if (fileVec[i].find("location /") && fileVec[i].find("location ."))
                    throw ("locations must have a path or extension file.");
                else if (fileVec[i][fileVec[i].length() - 1] != '{') {
                    if (fileVec[i + 1] != "{")
                        throw ("locations syntax error '{'");
                }
            }
            else if (!fileVec[i].find("Server")) {
                //std::cout << BLUE << "Server Found!" << RESET << std::endl;
                if (!(fileVec[i][fileVec[i].length() - 1] == '{' && (fileVec[i].length() == 8 || fileVec[i].length() == 7))
                    && !(fileVec[i][fileVec[i].length() - 1] == 'r' && fileVec[i].length() == 6)) {
                    throw ("Server Block symtax error '{'");
                }
                else if ((fileVec[i][fileVec[i].length() - 1] == 'r' && fileVec[i].length() == 6) && fileVec[i + 1] != "{") {
                    throw ("Server Block symtax error  '{'");
                }
            }
        }
        else if (fileVec[i][fileVec[i].length() - 1] != ';') {
                throw ("Configuration File, syntax error missing ';'");
        }
    }
}

size_t foundChar(std::string line, char c) {
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == c)
            return i;
    }
    return 0;
}

void    listenRule(std::string line) {
    consumeMultiSpaces(line);

    //std::cout << RED << line << RESET << std::endl;
    size_t pos = line.find(' ');
    size_t endpos = 0;
    size_t twoP = 0;
    if (line[line.length() - 2 == ' '])
        endpos = line.length() - 2;
    else
        endpos = line.length() - 1;
    std::string value;
    size_t countP = 0;
    //std::cout << endpos - pos << std::endl;
    if ((endpos - pos) > 6 && foundChar(line, ':')) {
        //std::cout << GREEN << "Entrei\n" << RESET;
        for (size_t i = (pos + 1); i < endpos - 1; i++) {
            if (line [i] == '.' || line[i] == ':' || isdigit(line[i])) {
                value += line[i];
                if (line[i] == '.') {
                    countP++;
                }
                if (line[i] == ':') {
                    twoP++;
                }
            }
            else 
                throw("Syntax error listen IP + PORT, must be positive numeric values (example: 127.0.0.1:8080)");
        }
        
        if (twoP != 1 || countP != 3) {
                throw("Syntax error listen IP + PORT, must be positive numeric values (example: 127.0.0.1:8080)");
        }
        twoP = foundChar(line, ':');
        //std::cout << twoP << std::endl;
        for (size_t i = (pos + 1); i < twoP; i++) {
            if (line[i] == '.') {
                //%%%%%%%%%%%%%
                //%%%%%%%%%%%%%
                //terminei AQUI
                //%%%%%%%%%%%%%
                //%%%%%%%%%%%%%
            }

        }
    } 
    else {
        for (int i = (pos + 1); line[i] != ';'; i++) {
            if(isdigit(line[i]))
                value += line[i];
            else if (line[i] == ' ')
                break ;
            else {
                throw ("Syntax error, listen Port is not a digit, must be a positive number");
            }
            //std::cout << BLUE << value << RESET << std::endl;
        }
        //std::cout << value << "$" << std::endl;
        if (value.empty()) {
            throw ("Syntax error, listen Porto doesnt contain value");
        }
        int port = atoi(value.c_str());
        if (port < 0 || port > 65535)
            throw ("listen Port must be a value between 0 - 65535");
        //std::cout << BLUE << line << " = " << port << std::endl; 
    }
    value.clear();
}

void    serverNameRule(std::string line) {
    int pos = line.find_first_of(' ');
    std::string value;
    for (int i = (pos + 1); line[i] != ';'; i++) {
        value += line[i];
    }
    if (value.empty()) {
        throw ("server_name cannot be empty");
    }
}

void    hostRule(std::string line) {
    size_t pos = line.find(' ');
    int countP = 0;
    std::string value;
    for (size_t i = (pos + 1); i < (line.length() - 1); i++) {
        if (line [i] == '.' || isdigit(line[i])) {
            value += line[i];
            if (line[i] == '.')
                countP++;
        }
        else
            throw("Syntax error host IP address, must be positive numeric values (example: 127.0.0.3)");
    }
    //std::cout << YELLOW << value << RESET << std::endl;
    if (value.empty()) {
        throw ("host IP address cannot be empty");
    }
    if (countP != 3 || value.length() < 7 || value.length() > 15) {
        std::cout << value.length() << " = " << countP << std::endl;
        std::cout << "[ " << RED << value << RESET << " ] is not a valid IP address" << std::endl;
        throw ("Syntax error host IP address");
    }
    int digit = 0;
    std::string v_digit;
    for (size_t i = 0; i < value.length(); i++) {
        if (value[i] == '.') {
            digit = atoi(v_digit.c_str());
            v_digit.clear();
            if (digit < 0 || digit > 255)
                throw ("Syntax error host IP address, IP address must be between 0.0.0.0 to 255.255.255.255");
        } else {
            v_digit += value[i];
            if ((i + 1) == value.length()) {
                digit = atoi(v_digit.c_str());
                v_digit.clear();
                if (digit < 0 || digit > 255)
                throw ("Syntax error host IP address, IP address must be between 0.0.0.0 to 255.255.255.255");
            }
        }
    }
}

void    verifyVar(std::vector<std::string> fileVec) {
    for (size_t i = 0; i < fileVec.size(); i++) {
        //std::cout << fileVec[i] << std::endl;
        if (!fileVec[i].find("listen"))
            listenRule(fileVec[i]);
        if (!fileVec[i].find("server_name"))
            serverNameRule(fileVec[i]);
        if (!fileVec[i].find("host"))
            hostRule(fileVec[i]);
            //std::cout << RED << "Entrei\n" << RESET;
    }
}


void    verifyValues(std::vector<std::string> fileVec) {
    std::vector<std::string> keyVec;
    keyVec = putKeys(keyVec);


    // Aqui verificamos as keywords se sao validas
    matchKeys(keyVec, fileVec);
    // Aqui varificamos se os blocos estao certos
    // se as locations estao corretamente dentro dos servers e os brackets estao certos tambem
    checkServerLocs(fileVec);
    verifyBlocks(fileVec);
    verifyVar(fileVec);

    keyVec.clear();
    fileVec.clear();
}

void cParser::getFileVec(std::string configurationFile) {
    std::ifstream file(configurationFile.c_str());
    //std::vector<std::string> fileVec;
    if (!file)
        throw ("Couldn't open file.");

    std::string line;
    //int i = 0;
    while (std::getline(file, line)) {
        line = consumeSpaces(line);
        line = consumeMultiSpaces(line);
/*         if (line.find('{')) {
            std::cout << BLUE << "yah - " << i++ << "\n" << RESET;
        } */
        if (!consumeComments(line) && !consumeEmptyLines(line))
            fileVec.push_back(line);
    }
    file.close();
    verifyValues(fileVec);
    fileVec.clear();
}