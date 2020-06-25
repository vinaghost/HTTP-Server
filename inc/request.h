#ifndef _REQUEST_INC
#define _REQUEST_INC

#include "header.h"

class Request : public Header {
    private:
        
        std::unordered_map<std::string, std::string> content;
        bool bGet;
        bool bPost;

        void parseData(const std::string &req);
        void parseMethod(const std::string &req);
    public:
        Request(const std::string &req);
        bool isPostMethod();
        bool isGetMethod();
        std::unordered_map<std::string, std::string> const &getContent() const;
        void showData();
};

#endif