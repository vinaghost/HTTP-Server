#include <iostream>
#include <exception>
#include <sstream>

class SocketError : public std::exception {
    protected:
        int WSA_Code;
        const char* error;
    public:
        SocketError(const char * error, int WSA_Code) : WSA_Code(WSA_Code), error(error) {};

        const char * what () const throw () {
            std::stringstream ss;
            ss << "Error: " << error << "\nWSA Code: " << WSA_Code; 
            char* ptr = new char[ss.str().length() + 1];
            strcpy(ptr, ss.str().c_str());
            return ptr;
        }
};

class SocketCreate : public SocketError {
    public:
        SocketCreate(int WSA_Code) : SocketError("create socket", WSA_Code) {};
};

class GetAddressinfo : public SocketError {
    public:
        GetAddressinfo(int WSA_Code) : SocketError("get address info", WSA_Code) {};
};

class SocketBind : public SocketError {
    public:
        SocketBind(int WSA_Code) : SocketError("bind socket", WSA_Code) {};    
};

class SocketListen : public SocketError {
    public:
        SocketListen(int WSA_Code) : SocketError("listen socket", WSA_Code) {};    
};

class SocketAccept : public SocketError {
    public:
        SocketAccept(int WSA_Code) : SocketError("accept socket", WSA_Code) {};    
};

class SocketReceive : public SocketError {
    public:
        SocketReceive(int WSA_Code) : SocketError("receive socket", WSA_Code) {};    
};

class SocketSend : public SocketError {
    public:
        SocketSend(int WSA_Code) : SocketError("send socket", WSA_Code) {};    
};

class SocketShutdown : public SocketError {
    public:
        SocketShutdown(int WSA_Code) : SocketError("shutdown socket", WSA_Code) {};    
};

