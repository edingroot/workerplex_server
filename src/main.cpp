#include <csignal>
#include <cstring>
#include <cstdint>
#include <iostream>
#include "apiserver/ApiServer.hpp"
#include "workerplex/Workerplex.hpp"
#include "workers/pingpong/PingPong.hpp"
#include "workers/longtask/LongTask.hpp"

using namespace std;

void configSignals();

void gotSignal(int);

atomic<bool> terminating(false);

int main() {
    configSignals();

    // Add workers
    Workerplex workerplex;
    workerplex.addWorker(new PingPong());
    workerplex.addWorker(new LongTask());

    // Configure http api server
    ApiServer apiServer("0.0.0.0", (uint16_t) 8080);
    apiServer.attachWorkerplex(workerplex);
    apiServer.start();

    // Stopping condition
    while (!terminating.load())
        sleep(1);
    cout << "Stopping server." << endl;
    apiServer.stop();

    return 0;
}

void configSignals() {
    struct sigaction sa{};

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = gotSignal;
    sigfillset(&sa.sa_mask);

    sigaction(SIGINT, &sa, nullptr);
}

void gotSignal(int) {
    terminating.store(true);
}
