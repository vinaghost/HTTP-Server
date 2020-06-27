#ifndef _REQUEST_INC
#define _REQUEST_INC

#include "header.h"

class Request : public Header {
    private:
        bool bGet;
        bool bPost;

        void parseData(const std::string &req);
        void parseMethod(const std::string &req);
    public:
        Request(const std::string &req);
        bool isPostMethod();
        bool isGetMethod();
        std::unordered_map<std::string, std::string> const &getHeader() const;
        std::unordered_map<std::string, std::string> const &getData() const;
        void showData();

};

#endif