#include "PingPong.hpp"
#include <iostream>

const string PingPong::IDENTIFIER = "ping";

string PingPong::getIdentifier() {
    return IDENTIFIER;
}

string PingPong::run(const vector<string> &args) {
    return "pong";
}
