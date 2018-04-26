#ifndef WORKERPLEX_LONGTASK_HPP
#define WORKERPLEX_LONGTASK_HPP

#include <string>
#include <vector>
#include "workerplex/Worker.hpp"

class LongTask : public Worker {
public:
    static const string IDENTIFIER;

    string getIdentifier() override;

    void run(const vector<string> &args) override;

};


#endif //WORKERPLEX_LONGTASK_HPP
