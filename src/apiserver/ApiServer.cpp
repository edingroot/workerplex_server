#include "ApiServer.hpp"
#include <cstdint>
#include <iostream>

// Added for handing json
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>


ApiServer::ApiServer(const string &listenAddress, uint16_t listenPort) : listenAddress(listenAddress),
                                                                         listenPort(listenPort) {
    settings = make_shared<Settings>();
    settings->set_bind_address(listenAddress);
    settings->set_port(listenPort);
    registerStaticRoutes();
}

void ApiServer::attachWorkerplex(Workerplex &workerplex) {
    this->workerplex = workerplex;

}

void ApiServer::start() {
    service.start(settings);
}

void ApiServer::stop() {
    service.stop();
}

void ApiServer::registerStaticRoutes() {
    auto resource = make_shared<Resource>();

    // GET /
    resource->set_path("/");
    resource->set_method_handler("GET", [] (const shared_ptr<Session> session) {
        const auto request = session->get_request();
        int content_length = request->get_header("Content-Length", 0);

        session->fetch(content_length, [request](const shared_ptr<Session> session, const Bytes &body) {
            fprintf(stdout, "GET /\n");

            string message = "Hello GET!";
            session->close(OK, message, {{"Content-Length", to_string(message.size())},
                                         {"Connection", "close"}});
        });
    });
    service.publish(resource);
}

ApiServer::~ApiServer() {
}
