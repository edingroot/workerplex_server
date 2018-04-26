#include "LongTask.hpp"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

const string LongTask::IDENTIFIER = "longtask";

string LongTask::getIdentifier() {
    return IDENTIFIER;
}

void LongTask::run(const vector<string> &args) {
    long duration = 0;

    if (!args.empty()) {
        try {
            duration = stol(args[0]);
        } catch (invalid_argument &e) {
            duration = 0;
        }
    }

    cout << "Running LongTask for " << duration << " milliseconds." << endl;
    boost::this_thread::sleep(boost::posix_time::millisec(duration));
    cout << endl << "LongTask done." << endl;
}
