#include "../includes/Utils.hpp"

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