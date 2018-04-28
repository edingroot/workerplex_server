#ifndef WORKERPLEX_SERVER_REQUESTPARSER_HPP
#define WORKERPLEX_SERVER_REQUESTPARSER_HPP

#include <string>
#include <map>
#include <vector>

using namespace std;

class RequestParser {
public:
    static map<string, string> parseFormUrlEncoded(string body);

    /**
     * @param arrayString e.g. [1, "a", "b", 2]
     */
    static vector<string> parseArgsArray(string arrayString);

};


#endif //WORKERPLEX_SERVER_REQUESTPARSER_HPP
