#ifndef WORKERPLEX_LONGTASK_HPP
#define WORKERPLEX_LONGTASK_HPP

#include <string>
#include "workerplex/Worker.hpp"

using namespace std;

class LongTask : public Worker {
public:
    static const string IDENTIFIER;

    string getIdentifier() override;

    string run(const vector<string> &args) override;

};


#endif //WORKERPLEX_LONGTASK_HPP
