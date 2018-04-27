#include "ApiServer.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include <boost/algorithm/string.hpp>
#include "RequestParser.hpp"

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

    // POST /run
    //  cmd=<command>
    //  args=["arg1", "arg2", ...]
    resource = make_shared<Resource>();
    resource->set_path("/run");
    resource->set_method_handler("POST", [](const shared_ptr<Session> session) {
        const auto request = session->get_request();
        int content_length = request->get_header("Content-Length", 0);

        session->fetch(static_cast<const size_t>(content_length),
                       [request](const shared_ptr<Session> session, const Bytes &body) {
            string bodyString(body.begin(), body.end());
            fprintf(stdout, "%s\n", bodyString.c_str());

            map<string, string> postData = RequestParser::parseFormUrlEncoded(bodyString);
            string command;
            vector<string> args;

            // Check and parse inputs
            auto itCmd = postData.find("cmd");
            auto itArgs = postData.find("args");

            if (itCmd != postData.end()) {
                command = itCmd->second;
            } else {
                // Cmd param not found
                session->close(BAD_REQUEST);
                return;
            }

            if (itArgs != postData.end()) {
                // Found args param
                args = RequestParser::parseArgsArray(itArgs->second);
            }

            fprintf(stdout, "POST /workerplex/run | cmd=%s, #args=%lu\n", command.c_str(), args.size());

            // TODO: run command

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
    resource->set_method_handler("GET", [](const shared_ptr<Session> session) {
        const auto request = session->get_request();
        int content_length = request->get_header("Content-Length", 0);

        session->fetch(static_cast<const size_t>(content_length),
                       [request](const shared_ptr<Session> session, const Bytes &body) {
            fprintf(stdout, "GET /\n");

            string message = "A Workerplex Server";
            session->close(OK, message, {{"Content-Length", to_string(message.size())},
                                         {"Connection",     "close"}});
        });
    });
    service.publish(resource);

    return true;
}

ApiServer::~ApiServer() {
}
