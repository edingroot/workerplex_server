#include "RequestParser.hpp"
#include <cstdio>
#include <sstream>
#include <restbed>
#include <boost/algorithm/string.hpp>

// Added for handing json
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

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

vector<string> RequestParser::parseArgsArray(string arrayString) {
    vector<string> args;

    try {
        using namespace boost::property_tree;
        stringstream ss;
        ptree pt;

        ss << "{\"a\":" << arrayString << "}";
        read_json(ss, pt);

        for (boost::property_tree::ptree::value_type &v : pt.get_child("a")) {
            // Array elements should have no names
            if (v.first.empty()) {
                args.emplace_back(v.second.data());
            } else {
                break;
            }
        }

    } catch (exception const& e) {
        cerr << "[RequestParser::parseArgsArray] " << e.what() << endl;
    }

    return args;
}
