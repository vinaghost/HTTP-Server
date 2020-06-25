#ifndef _RESPONSE_INC
#define _RESPONSE_INC

#include "header.h" 

class Response : public Header {
    public:
        Response(unsigned long int size);
        std::string getResponse();

};

#endif