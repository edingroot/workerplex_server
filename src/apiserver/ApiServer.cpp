#include "ApiServer.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include "workerplex/Exceptions.hpp"
#include "RequestParser.hpp"

ApiServer::ApiServer(const string &listenAddress, uint16_t listenPort) : listenAddress(listenAddress),
                                                                         listenPort(listenPort) {
    settings = make_shared<Settings>();
    settings->set_bind_address(listenAddress);
    settings->set_port(listenPort);
    settings->set_connection_timeout(chrono::seconds(60));
    registerStaticRoutes();
}

bool ApiServer::attachWorkerplex(Workerplex &workerplex) {
    if (started)
        return false;

    shared_ptr<Resource> resource;
    this->workerplex = workerplex;

    // POST /run
    //  cmd=<command>
    //  args=[arg1, "arg2", ...]
    resource = make_shared<Resource>();
    resource->set_path("/run");
    resource->set_method_handler("POST", [this](const shared_ptr<Session> session) {
        const auto request = session->get_request();
        int content_length = request->get_header("Content-Length", 0);

        session->fetch(static_cast<const size_t>(content_length),
                       [&, this, request](const shared_ptr<Session> session, const Bytes &body) {
            string bodyString(body.begin(), body.end());
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
                string message = "post param:cmd not found";
                fprintf(stderr, "400 POST /workerplex/run | %s\n", message.c_str());
                session->close(BAD_REQUEST, message, {{"Content-Length", to_string(message.size())},
                                                      {"Connection", "close"}});
                return;
            }

            if (itArgs != postData.end()) {
                // Found args param
                args = RequestParser::parseArgsArray(itArgs->second);
            }

            // Run command
            try {
                string response = this->workerplex.runCommandSync(command, args);

                fprintf(stdout, "200 POST /workerplex/run | cmd=%s, #args=%lu\n", command.c_str(), args.size());
                session->close(OK, response, {{"Content-Length", to_string(response.size())},
                                              {"Connection", "close"}});
            } catch (command_not_found &e) {
                string message = string(e.what());

                fprintf(stderr, "400 POST /workerplex/run | cmd=%s, #args=%lu | %s\n", command.c_str(), args.size(), message.c_str());
                session->close(BAD_REQUEST, message, {{"Content-Length", to_string(message.size())},
                                                      {"Connection", "close"}});
            } catch (exception &e) {
                string message = string(e.what());

                fprintf(stderr, "500 POST /workerplex/run | cmd=%s, #args=%lu | %s\n", command.c_str(), args.size(), message.c_str());
                session->close(BAD_REQUEST, message, {{"Content-Length", to_string(message.size())},
                                                      {"Connection", "close"}});
            }
        });
    });
    service.publish(resource);

    return true;
}

void ApiServer::start() {
    started = true;
    cout << "API Server listening on " << settings->get_bind_address() << ":" << settings->get_port() << endl;
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
