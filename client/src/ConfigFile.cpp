#include "../includes/ConfigFile.hpp"

//--- CONSTRUCTORS && DESTRUCTOR ---
ConfigFile::ConfigFile(std::string conFile) : _ip_address(""), _port(0), _root(""), _index(""), _error_page("")
{
    parser(conFile);
}

ConfigFile::~ConfigFile() 
{

    _servers.clear();
    _locations.clear();

}
ConfigFile::ConfigFile() {
}
ConfigFile::ConfigFile(const ConfigFile &src) { (void)src; }


int ConfigFile::getPort() const
{
    return this->_port;
}

std::string ConfigFile::getIpAddr() const
{
    return this->_ip_address;
}

std::string ConfigFile::getRoot() const
{
    return this->_root;
}

std::string ConfigFile::getIndex() const
{
    return this->_index;
}

std::string ConfigFile::getErrorPage() const
{
    return this->_error_page;
}

std::vector<Server> &ConfigFile::getServers()
{
    return _servers;
}


void ConfigFile::setIpAddr(std::string set_ip_addr)
{
    this->_ip_address = set_ip_addr;
}

void ConfigFile::setIndex(std::string set_index)
{
    this->_index = set_index;
}

void ConfigFile::setRoot(std::string set_root)
{
    this->_root = set_root;
}

void ConfigFile::setErrorPage(std::string set_error_page)
{
    this->_error_page = set_error_page;
}

std::string ft_trim(const std::string &line)
{
    size_t startPos = line.find_first_not_of(" \t");
    if (startPos != std::string::npos)
    {
        return line.substr(startPos);
    }
    else
    {
        return "";
    }
}

void ConfigFile::parser(std::string &conFile)
{
    std::ifstream readConFile(conFile.c_str());
    std::string line;
    int Brackets = 0;
    if (!readConFile.is_open())
    {
        std::cout << "Error: ConFile cannot be opened.(Parser)\n";
        return;
    }

    std::stringstream serverContent;
    std::stringstream locationContent;
    bool insideServerBlock = false;
    bool insideLocationBlock = false;
    std::string path;

    while (std::getline(readConFile, line))
    {
        std::stringstream ss(line);
        std::string fword;
        ss >> fword;
        if (fword.empty() || fword.at(0) == ';' || fword.at(0) == '#')
            continue;

        if (fword == "Server")
        {
            std::string sword;
            ss >> sword;
            if (insideServerBlock)
            {
                log("ERROR: Server inside Server");
                exit(1);
            }
            if (sword != "{")
            {
                log("ERROR: Invalid configure file, {");
                exit(1);
            }
            Brackets++;
            insideServerBlock = true;
        }
        else if (fword == "location")
        {
            if (!insideServerBlock)
            {
                log("ERROR: Invalid configure file, { outside");
                exit(1);
            }
            std::string sword;

            ss >> sword;

            if (sword.empty() || ( (sword.at(0) != '/') && (sword.at(0) != '.')))
            {
                log("ERROR: Invalid location patssh");
                exit(1);
            }
            path = sword;
            std::string tword;
            ss >> tword;

            if (tword != "{")
            {
                log("ERROR: Invalid location path");
                exit(1);
            }
            else if (tword == "{")
            {
                insideLocationBlock = true;
                Brackets++;
            }
            else if (tword.empty())
            {
                while (std::getline(readConFile, line))
                {
                    if (fword.empty() || fword.at(0) == ';' || fword.at(0) == '#')
                        continue;

                    if (line.find('{') != std::string::npos)
                    {
                        Brackets++;
                        insideLocationBlock = true;
                        break;
                    }
                    if (line.find_first_not_of(" \t") != std::string::npos)
                    {
                        log("Error, not find '{' ");
                        exit(1);
                    }
                }
            }
        }
        else if (fword == "}" && insideLocationBlock == true)
        {
            Brackets--;
            insideLocationBlock = false;
            parserLocation(path, &locationContent);
            locationContent.str("");
            locationContent.clear();
        }
        else if (fword == "}")
        {
            if (!insideServerBlock)
            {
                log("ERROR: Invalid configure file, { outside");
                exit(1);
            }
            Brackets--;
            if (Brackets == 0)
            {
                insideServerBlock = false;
                parserServer(serverContent);
                serverContent.str("");
                serverContent.clear();
            }
        }
        else
        {
            if (insideServerBlock)
            {
                if (line.find('{') != std::string::npos)
                    Brackets++;
            }
        }
        if (insideLocationBlock == false && insideServerBlock == true)
        {
            serverContent << line << "\n";
        }
        if (insideLocationBlock == true && insideServerBlock == true)
        {
            locationContent << line << "\n";
        }
    }
    readConFile.close();

}

bool ConfigFile::parserServer(std::stringstream &serverContent)
{
    std::string line;
    Server server;

    while (std::getline(serverContent, line))
    {
        std::string trimLine = ft_trim(line);
        if (trimLine.empty())
        {
            continue;
        }
        std::stringstream ss(line);
        std::string fword;
        ss >> fword;
        if (fword == "listen")
        {
            std::string temp_port = obtainValue(line, "listen");
            std::size_t find = temp_port.find(':');
            if (find == std::string::npos)
            {
                server.setPort_s(temp_port);
                server.sin_port = htons(ft_stoi(server.getPort_s()));
                server.s_host = "127.0.0.1";
                server.s_addr = htonl(str_to_uint32(server.s_host));
            }
            else
            {
                server.setPort_s((temp_port.substr(find + 1)));
                server.sin_port = htons(ft_stoi(server.getPort_s()));
                server.s_host = temp_port.substr(0 , find);
                server.s_addr = htonl(str_to_uint32(server.s_host));
                server.setIpAddr_s(server.s_host);
            }
        }
        if (fword == "client_max_body_size")
            server.setClientMaxBody(obtainPort(line, "client_max_body_size"));
        if (fword == "host")
        {
            server.setIpAddr_s(obtainValue(line, "host"));
            server.s_host = server.getIpAddr_s();
            server.s_addr = htonl(str_to_uint32(server.s_host));
        }
        if (fword == "root")
            server.setRoot_s(obtainValue(line, "root"));
        if (fword == "index")
            server.setIndex_s(obtainValue(line, "index"));
        if (fword == "error_page")
            server.setErrorPage_s(obtainValue(line, "error_page"));
        if (fword == "server_name")
        {
            std::string temp_serv_name = obtainValue(line, "server_name");
            std::string token;
            //std::cout  << CYAN << "SERVER  :" << temp_serv_name << RESET << std::endl;
            for (size_t i = 0; i < temp_serv_name.size(); ++i)
            {

                char c = temp_serv_name[i];
                if (c == ' ' || c == '\t')
                {

                    if (!token.empty())
                    {
                        server.s_server_names.push_back(token);
                        token.clear();                    
                    }
                }
                else
                {
                    token += c;
                }
            }
            if (!token.empty()) {
        server.s_server_names.push_back(token);
    }
    }
    }
    server.setLocation(_locations);
    server.verificErrorServer();
    _locations.clear();
    _servers.push_back(server);
    //printVector(server.s_server_names);
    if (_servers.empty())
    {
        std::cout << "No servers found in the vector.\n";
        return false;
    }


    return true;
}

// Parser da Location, fiz bem parecido com o que temos para o server, para poder reutilizar o codigo
bool ConfigFile::parserLocation(std::string path, std::stringstream *locationContent)
{
    std::string line;
    Location location;

    while (std::getline(*locationContent, line))
    {
        std::string trimLine = ft_trim(line);
        if (trimLine.empty())
        {
            continue;
        }
        std::stringstream ss(line);
        std::string fword;
        ss >> fword;
        if (fword == "allow_methods")
        {
            std::vector<std::string> methods_vec;
            std::string str_methods = obtainValue(line, "allow_methods");
            int flag = 0;
            int start = 0;
            for (size_t i = 0; i < str_methods.length() + 1; i++)
            {
                if (str_methods[i] && !flag)
                {

                    start = i;
                    flag++;
                }
                if ((str_methods[i] == ' ' || str_methods[i] == '\0') && flag)
                {
                    flag--;
                    methods_vec.push_back(str_methods.substr(start, i - start));
                }
            }
            location.setAllowMethods(methods_vec);
        }
        if (fword == "upload_to")
            location.setUploadTo(obtainValue(line, "upload_to"));
        if (fword == "cgi_path")
            location.setCgiPath(obtainValue(line, "cgi_path"));
        if (fword == "cgi_ext")
            location.setCgiExt(obtainValue(line, "cgi_ext"));
        if (fword == "autoindex")
            location.setAutoIndex(obtainValue(line, "autoindex"));
        if (fword == "return")
        {
            location.setReturn(obtainValue(line, "return"));
        }
        if (fword == "root")
        {
            location.setRoot(obtainValue(line, "root"));
        }
    }
    location.setPath(path);

    _locations.push_back(location);
    if (_locations.empty())
    {
        std::cout << "No Locations found in the vector.\n";
        return false;
    }

    return true;
}

std::string ConfigFile::obtainValue(const std::string &line, const std::string &type)
{
    size_t pos = line.find(type);
    if (pos != std::string::npos)
    {
        pos += type.size();
        pos = line.find_first_not_of(" \t", pos);
        if (pos != std::string::npos)
        {
            size_t end_pos = line.find(';', pos);
            if (end_pos != std::string::npos)
            {
                end_pos = line.find_last_not_of(" \t", end_pos - 1);
                if (end_pos != std::string::npos)
                {
                    return line.substr(pos, end_pos - pos + 1);
                }
                else
                {
                    return line.substr(pos, end_pos);
                }
            }
            else
            {
                exitWithError("missing ;");
                exit(EXIT_FAILURE);
            }
        }
    }
    return "";
}

int ConfigFile::obtainPort(std::string line, std::string token)
{
    std::string port = obtainValue(line, token);
    return (port.empty()) ? 0 : std::atoi(port.c_str());
}

std::string ConfigFile::ft_trim(const std::string &line)
{
    size_t startPos = line.find_first_not_of(" \t");
    if (startPos != std::string::npos)
    {
        return line.substr(startPos);
    }
    else
    {
        return "";
    }
}

void ConfigFile::clearConf()
{
    _locations.clear();
    _servers.clear();
}