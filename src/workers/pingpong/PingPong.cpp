#include "PingPong.hpp"

const string PingPong::IDENTIFIER = "ping";

string PingPong::getIdentifier() {
    return IDENTIFIER;
}

void PingPong::run(const vector<string> &args) {
    cout << "pong" << endl;
}
