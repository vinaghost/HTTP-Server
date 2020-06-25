#ifndef _HEADER_INC
#define _HEADER_INC


#include <iostream>
#include <string>
#include <unordered_map>

class Header {

    protected:
        std::unordered_map<std::string, std::string> mData;
    public:
        Header() = default;

        virtual void showData();

};

#endif