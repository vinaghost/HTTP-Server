#ifndef _REQUEST_INC
#define _REQUEST_INC

#include "header.h"

class Request : public Header {
    private:
        std::unordered_map<std::string, std::string> content;

        void parseData(const std::string &req);
        void parseMethod(const std::string &req);
    public:
        Request(const std::string &req);
        void showData();
};

#endif