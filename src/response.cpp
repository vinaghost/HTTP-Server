#include "response.h"
#include <sstream>

Response::Response(unsigned long int size) {
    mData.insert(std::make_pair("Status", "HTTP/1.1 200 OK"));
    mData.insert(std::make_pair("Connection", "close"));
    mData.insert(std::make_pair("Content-Type", "text/html"));
    char sSize[10];
    sprintf_s(sSize, sizeof(sSize), "%ld", size);
    mData.insert(std::make_pair("Content-Length", sSize));
}

std::string Response::getResponse() {
    std::stringstream ss;

    ss << mData["Status"] << "\r\n";
    ss << "Connection: " << mData["Connection"] << "\r\n"; 
    ss << "Content-Type: " << mData["Content-Type"] << "\r\n"; 
    ss << "Content-Length: " << mData["Content-Length"] << "\r\n\r\n";

    return std::string(ss.str());
}