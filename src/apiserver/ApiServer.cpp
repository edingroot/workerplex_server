#include "ApiServer.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include "RequestParser.hpp"

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

bool ApiServer::attachWorkerplex(Workerplex &workerplex) {
    if (started)
        return false;

    shared_ptr<Resource> resource;
    this->workerplex = workerplex;

    // POST /workerplex/run
    //  cmd="<command>"
    //  args="[arg1, arg2, ...]"
    resource = make_shared<Resource>();
    resource->set_path("/workerplex/run");
    resource->set_method_handler("POST", [] (const shared_ptr<Session> session) {
        const auto request = session->get_request();
        int content_length = request->get_header("Content-Length", 0);

        session->fetch(content_length, [request](const shared_ptr<Session> session, const Bytes &body) {
            string bodyString(body.begin(), body.end());
            fprintf(stdout, "%s\n", bodyString.c_str());

            map<string, string> postData = RequestParser::parseFormUrlEncoded(bodyString);
            // TODO: check if key exists, otherwise return 400
            string command = postData.find("cmd")->second.c_str();
            fprintf(stdout, "POST /workerplex/run  cmd=%s\n", command.c_str());

            // TODO: read command args

            session->close(OK, bodyString, {{"Content-Length", to_string(bodyString.size())},
                                         {"Connection", "close"}});
        });
    });
    service.publish(resource);

    return true;
}

void ApiServer::start() {
    started = true;
    service.start(settings);
}

void ApiServer::stop() {
    service.stop();
    started = false;
}

bool ApiServer::registerStaticRoutes() {
    if (started)
        return false;

    shared_ptr<Resource> resource;

    // GET /
    resource = make_shared<Resource>();
    resource->set_path("/");
    resource->set_method_handler("GET", [] (const shared_ptr<Session> session) {
        const auto request = session->get_request();
        int content_length = request->get_header("Content-Length", 0);

        session->fetch(content_length, [request](const shared_ptr<Session> session, const Bytes &body) {
            fprintf(stdout, "GET /\n");

            string message = "A Workerplex Server";
            session->close(OK, message, {{"Content-Length", to_string(message.size())},
                                         {"Connection", "close"}});
        });
    });
    service.publish(resource);

    return true;
}

ApiServer::~ApiServer() {
}
