#include "LongTask.hpp"
#include <sstream>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

const string LongTask::IDENTIFIER = "longtask";

string LongTask::getIdentifier() {
    return IDENTIFIER;
}

string LongTask::run(const vector<string> &args) {
    long duration = 0;

    if (!args.empty()) {
        try {
            duration = stol(args[0]);
        } catch (invalid_argument &e) {
            duration = 0;
        }
    }

    stringstream sout;
    sout << "Running LongTask for " << duration << " milliseconds." << endl;
    boost::this_thread::sleep(boost::posix_time::millisec(duration));
    sout << endl << "LongTask done." << endl;

    return sout.str();
}
