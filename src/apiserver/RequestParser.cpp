#include "RequestParser.hpp"
#include <restbed>
#include <boost/algorithm/string.hpp>
#include <cstdio>

map<string, string> RequestParser::parseFormUrlEncoded(string body) {
    map<string, string> data;
    vector<string> tokens;

    boost::split(tokens, body, boost::is_any_of("&"));
    for (string &token : tokens) {
        vector<string> nameValuePair;
        boost::split(nameValuePair, token, boost::is_any_of("="));

        string key = nameValuePair[0];
        string value = nameValuePair.size() > 1 ? nameValuePair[1] : "";

        data[restbed::Uri::decode_parameter(key)] = restbed::Uri::decode_parameter(value);
    }

    return data;
}
