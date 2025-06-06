#ifndef AUTHORIZE_REQUEST_HANDLER_HPP
#define AUTHORIZE_REQUEST_HANDLER_HPP

#include <fstream>
#include <memory>
#include <string>

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTPRequests.hpp>
#include <Network/URL.hpp>

#include "database.hpp"
#include "jwt.hpp"

namespace handlers
{
    net::HTTPResponse authorize(net::HTTPRequest request)
    {
        if (request.start_line[0] == "GET")
        {
            std::string token = request.headers["Authorization"];
            if ()
        }

        if (request.start_line[0] == "POST")
        {
            if db::exec("SELECT id FROM users WHERE login_hash = \"" + uri.getParamsPtr()["login"] + "\";");
        }
        net::HTTPResponse response;
        net::URI uri(request.start_line[1]);

        std::shared_ptr<jst::JSArray> result = db::exec("SELECT id FROM users WHERE login_hash = \"" + uri.getParamsPtr()["login"] + "\" AND password_hash = \"" + uri.getParamsPtr()["password"] + "\";");
        std::cout << "AUTH: " << result->toString();
        if (result->undefined == true)
        {
            response.start_line[1] = "401";
            response.start_line[2] = "UNAUTHORIZED";
        }
        else
        {
            response.start_line[1] = "200";
            response.start_line[2] = "OK";
            response.headers["Content-Type"] = "application/javascript; charset=utf-8";

            response.body = result->operator[](0)->toString();
        }

        response.start_line[0] = "HTTP/1.1";
        response.headers["Version"] = "HTTP/1.1";
        response.headers["Access-Control-Allow-Origin"] = "*";
        response.headers["Content-Length"] = std::to_string(response.body.length());

        return response;
    }
}

#endif //AUTHORIZE_REQUEST_HANDLER_HPP