#ifndef WORKERPLEX_APISERVER_HPP
#define WORKERPLEX_APISERVER_HPP

#include <string>
#include <restbed>
#include "workerplex/Workerplex.hpp"

using namespace std;
using namespace restbed;

class ApiServer {
public:
    ApiServer(const string &listenAddress, uint16_t listenPort);

    bool attachWorkerplex(Workerplex &workerplex);

    void start();

    void stop();

    virtual ~ApiServer();

private:
    const string listenAddress;
    const uint16_t listenPort;
    bool started = false;

    Service service;
    shared_ptr<Settings> settings;
    Workerplex workerplex;

    bool registerStaticRoutes();

};


#endif //WORKERPLEX_APISERVER_HPP
