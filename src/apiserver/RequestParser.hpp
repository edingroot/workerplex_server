#ifndef WORKERPLEX_SERVER_REQUESTPARSER_HPP
#define WORKERPLEX_SERVER_REQUESTPARSER_HPP

#include <map>
#include <string>

using namespace std;

class RequestParser {
public:
    static map<string, string> parseFormUrlEncoded(string body);

};


#endif //WORKERPLEX_SERVER_REQUESTPARSER_HPP
