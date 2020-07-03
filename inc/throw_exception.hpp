#include <iostream>
#include <exception>
#include <sstream>

////////////////////////////////////////////////////////////
/// \brief Interface của các class exception 
///
////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Create
///
////////////////////////////////////////////////////////////
class SocketCreate : public SocketError {
    public:
        SocketCreate(int WSA_Code) : SocketError("create socket", WSA_Code) {};
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: GetAddressinfo
///
////////////////////////////////////////////////////////////
class GetAddressinfo : public SocketError {
    public:
        GetAddressinfo(int WSA_Code) : SocketError("get address info", WSA_Code) {};
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Bind
///
////////////////////////////////////////////////////////////
class SocketBind : public SocketError {
    public:
        SocketBind(int WSA_Code) : SocketError("bind socket", WSA_Code) {};    
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Listen
///
////////////////////////////////////////////////////////////
class SocketListen : public SocketError {
    public:
        SocketListen(int WSA_Code) : SocketError("listen socket", WSA_Code) {};    
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Accept
///
////////////////////////////////////////////////////////////
class SocketAccept : public SocketError {
    public:
        SocketAccept(int WSA_Code) : SocketError("accept socket", WSA_Code) {};    
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Receive
///
////////////////////////////////////////////////////////////
class SocketReceive : public SocketError {
    public:
        SocketReceive(int WSA_Code) : SocketError("receive socket", WSA_Code) {};    
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Send
///
////////////////////////////////////////////////////////////
class SocketSend : public SocketError {
    public:
        SocketSend(int WSA_Code) : SocketError("send socket", WSA_Code) {};    
};

////////////////////////////////////////////////////////////
/// \brief Excpetion class: Socket Shutdown
///
////////////////////////////////////////////////////////////
class SocketShutdown : public SocketError {
    public:
        SocketShutdown(int WSA_Code) : SocketError("shutdown socket", WSA_Code) {};    
};

