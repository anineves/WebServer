#ifndef CPARSER_HPP
# define CPARSER_HPP
# include "../includes/webServer.hpp"
# include <vector>

class cParser {
    public:
        // ==== Variables ====
        std::vector<std::string> fileVec;
    
        // ==== Functions =====
        cParser();
        ~cParser();
        void    getFileVec(std::string fileVec);
};



#endif