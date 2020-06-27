#include <sstream>
#include "request.h"
#include "boost/algorithm/string.hpp"

Request::Request(const std::string &req) :
    bPost(false),
    bGet(false)
{
    parseData(req);
}
void Request::parseData(const std::string &data) {
    std::istringstream request(data);
    std::string header;
    std::string::size_type index;

    std::getline(request, header);
    std::stringstream tmp(header);
    std::string method, req_url, status;
    tmp >> method >> req_url >> status;

    mHeader.insert(std::make_pair("Method", method));
    mHeader.insert(std::make_pair("Request Url", req_url.c_str() + 1));
    mHeader.insert(std::make_pair("Status", status));


    while (std::getline(request, header) && header != "\r") {
        index = header.find(':', 0);
        if(index != std::string::npos) {
            mHeader.insert(std::make_pair(
                boost::algorithm::trim_copy(header.substr(0, index)), 
                boost::algorithm::trim_copy(header.substr(index + 1))
            ));
        }
    }
    
    if( mHeader["Method"] == "GET") {
        tmp.clear();
        tmp.str(mHeader["Request Url"]);
        
        std::string url; 
        std::getline(tmp, url, '?');
        mHeader["Request Url"].assign(url);

        std::string req;
        tmp >> req;
        parseMethod(req);

        bGet = true;
    }
    else if ( mHeader["Method"] == "POST") {
        std::getline(request, header);
        parseMethod(header);

        bPost = true;
    }
}

bool Request::isGetMethod() {
    return bGet;
}

bool Request::isPostMethod() {
    return bPost;
}

std::unordered_map<std::string, std::string> const &Request::getHeader() const {
    return mHeader;
}
std::unordered_map<std::string, std::string> const &Request::getData() const {
    return mData;
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
            mData.insert(std::make_pair(request.substr(0, pos), request.substr(pos + 1)));
        }
        ss >> request;
    }
}
void Request::showData() {
    std::cout << "[Request]";
    Header::showData();
}