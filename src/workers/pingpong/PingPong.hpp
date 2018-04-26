#ifndef HANDRECOGNIZER_PINGPONG_HPP
#define HANDRECOGNIZER_PINGPONG_HPP

#include <iostream>
#include "workerplex/Worker.hpp"

using namespace std;

class PingPong : public Worker {
public:
    static const string IDENTIFIER;

    string getIdentifier() override;

    void run(const vector<string> &args) override;

};


#endif //HANDRECOGNIZER_PINGPONG_HPP
