#include "response.h"

void Response::set(unsigned int code ) {
    Response::code = code;
}

void Response::set(unsigned int code, unsigned long int size) {
    set(code);

    if ( code == 200 ) {
        mHeader.insert(std::make_pair("Status", "HTTP/1.1 200 OK"));
        mHeader.insert(std::make_pair("Connection", "close"));
        mHeader.insert(std::make_pair("Content-Type", "text/html"));
        char sSize[10];
        sprintf_s(sSize, sizeof(sSize), "%ld", size);
        mHeader.insert(std::make_pair("Content-Length", sSize));
    }  
    else if (code  == 404) {
        mHeader.insert(std::make_pair("Status", "HTTP/1.1 404 Not found"));

        mHeader.insert(std::make_pair("Connection", "close"));
        mHeader.insert(std::make_pair("Content-Type", "text/html"));

        char sSize[10];
        sprintf_s(sSize, sizeof(sSize), "%ld", size);
        mHeader.insert(std::make_pair("Content-Length", sSize));

    }  
    createResponse();

}
void Response::set(unsigned int code, const std::string &content) {
    set(code);

    if ( code == 301) {
        mHeader.insert(std::make_pair("Status", "HTTP/1.1 301 Moved Permanently"));
        mHeader.insert(std::make_pair("Location", content));
        
    }

    createResponse();
}
void Response::reset() {
    code = 0;
    mHeader.empty();
    mData.empty();
    ss.clear();
    ss.str("");
}

void Response::createResponse() {
    ss.str("");

    if( code == 200 || code == 404) {
        ss << mHeader["Status"] << "\r\n";
        ss << "Connection: " << mHeader["Connection"] << "\r\n"; 
        ss << "Content-Type: " << mHeader["Content-Type"] << "\r\n"; 
        ss << "Content-Length: " << mHeader["Content-Length"] << "\r\n\r\n";

    }
    else if ( code == 301) {
        ss << mHeader["Status"] << "\r\n";
        ss << "Location: /" << mHeader["Location"] << "\r\n\r\n."; 

    }
    
}
std::string Response::getResponse() {
    return ss.str();
}

void Response::showData() {
    std::cout << "[Response]";
    Header::showData();
}