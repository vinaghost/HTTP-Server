#ifndef _RESPONSE_INC
#define _RESPONSE_INC

#include <sstream>
#include "header.h" 
class Response : public Header {
    private:
        std::stringstream ss;
    public:
        unsigned int code;
        void set(unsigned int code);
        void set(unsigned int code, unsigned long int size);
        void set(unsigned int code, const std::string& content);
        void reset();
        std::string getResponse();
        void showData();


};

#endif