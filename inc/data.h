#ifndef _DATA_INC
#define _DATA_INC

#include <string>

class Data {    
    protected:
        enum class DATA_TYPE {
            HEADER,
            INPUT,
            OUTPUT
        };
        DATA_TYPE type;
        std::string data;
    public:
        Data(std::string data, DATA_TYPE type);
        Data() = delete;
};

#endif