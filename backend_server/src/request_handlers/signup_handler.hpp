#ifndef SIGNUP_HANDLER_HPP
#define SIGNUP_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <utility>
#include <string>
#include <memory>

#include "generateJWT.hpp"
#include "database.hpp"
#include "jwt.hpp"


namespace handlers
{
    net::HTTPResponse signup(net::HTTPRequest request)
    {
        net::HTTPResponse response;
        net::URI uri(request.start_line[1]);

    	if (request.start_line[0] == "POST")
        {
            std::string login = uri.getParamsPtr()["login"];
            std::string password = uri.getParamsPtr()["password"];
            std::shared_ptr<jst::JSArray> result = db::exec("SELECT id FROM users WHERE login_hash = \"" + login + "\";");
            if (result->size() > 0)
            {
                response.start_line[1] = "409";
                response.start_line[2] = "CONFLICT";
            }
            else
            {
                std::string id = "id" + login;
                std::pair<std::string, std::string> tokens = generateJWT(id, 60, 60*3);

                jst::JSObject json;
                json.addField("access_token", std::make_shared<jst::JSString>(tokens.first));
                json.addField("refresh_token", std::make_shared<jst::JSString>(tokens.second));

                db::exec("INSERT INTO users (id, login_hash, password_hash, refresh_token)\
                          VALUES (\"" + login + "\", \"" + login + "\", \"" + password + "\", \"" + tokens.second + "\");");

                response.start_line[1] = "200";
                response.start_line[2] = "OK";
                response.body = json.toString();
            }
        }
        else
        {
            response.start_line[1] = "405";
            response.start_line[2] = "METHOD NOT ALLOWED";
        }

        response.start_line[0] = "HTTP/1.1";
        response.headers["Version"] = "HTTP/1.1";
        response.headers["Access-Control-Allow-Origin"] = "*";
        response.headers["Content-Length"] = std::to_string(response.body.length());

        return response;
    }
}

#endif //SIGNUP_HANDLER_HPP