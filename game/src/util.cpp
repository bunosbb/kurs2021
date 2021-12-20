#include "util.h"

POINT::POINT() {
    x = 0;
    y = 0;
}

POINT::POINT(int n_x, int n_y){
    x = n_x;
    y = n_y;
}

POINT::operator std::pair<int32_t, int32_t> () {
    return std::make_pair(x, y);
}


std::ofstream Logger::log = std::ofstream("../log/log.txt");

Logger::Logger() {}

void Logger::operator() (std::string clas, std::string func, std::string message) {
    log << clas << ", " << func << ": " << message << std::endl;
    //std::cout << clas << ", " << func << ": " << message << '\n';
}