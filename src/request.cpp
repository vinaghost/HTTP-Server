#include <sstream>
#include "request.h"
#include "boost/algorithm/string.hpp"

Request::Request(const std::string &req) {
    parseData(req);
}
void Request::showData() {
    Header::showData();
    std::cout << "\n==========Content==============\n";
    for( auto &i : content) {
        std::cout << "] " << i.first << " - " << i.second << "\n";
    }
}
void Request::parseData(const std::string &data) {
    std::istringstream request(data);
    std::string header;
    std::string::size_type index;

    // HTTP/1.1
    std::getline(request, header);
    std::stringstream tmp(header);
    std::string method, req_url, status;
    tmp >> method >> req_url >> status;

    mData.insert(std::make_pair("Method", method));
    mData.insert(std::make_pair("Request Url", req_url.c_str() + 1));
    mData.insert(std::make_pair("Status", status));


    while (std::getline(request, header) && header != "\r") {
        index = header.find(':', 0);
        if(index != std::string::npos) {
            mData.insert(std::make_pair(
                boost::algorithm::trim_copy(header.substr(0, index)), 
                boost::algorithm::trim_copy(header.substr(index + 1))
            ));
        }
    }
    
    if( mData["Method"] == "GET") {
        tmp.clear();
        tmp.str(mData["Request Url"]);
        
        std::string url; 
        std::getline(tmp, url, '?');
        mData["Request Url"].assign(url);

        std::string req;
        tmp >> req;
        parseMethod(req);
    }
    else if ( mData["Method"] == "POST") {
        std::getline(request, header);
        parseMethod(header);
    }
}

void Request::parseMethod(const std::string &req) {
    std::string request(req);
    for (auto &i : request) {
        if ( i == '&') {
            i = ' ';
        }
    }

    std::istringstream ss(request);
    ss >> request;

    std::string::size_type pos;
    while (ss) {
        pos = request.find_first_of("=");
        if (pos != std::string::npos) {
            content.insert(std::make_pair(request.substr(0, pos), request.substr(pos + 1)));
        }
        ss >> request;
    }
}