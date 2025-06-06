#ifndef CREATE_TOKEN_HANDLER_HPP
#define CREATE_TOKEN_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <string>
#include <memory>
#include <ctime>

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
                response.start_line[1] = "200";
                response.start_line[2] = "OK";

                jst::JSObject payload;
                payload.addField("exp", std::make_shared<jst::JSNumber>(time(NULL) + 60));
                std::string access_token = jwt::createToken(payload);

                payload.removeField("exp");
                payload.addField("exp", std::make_shared<jst::JSNumber>(time(NULL) + 3*60));
                std::string refresh_token = jwt::createToken(payload);

                jst::JSObject json;
                json.addField("access_token", std::make_shared<jst::JSString>(access_token));
                json.addField("refresh_token", std::make_shared<jst::JSString>(refresh_token));

                db::exec("INSERT INTO users (id, login_hash, password_hash, refresh_token)\
                          VALUES (\"" + login + "\", \"" + login + "\", \"" + password + "\", \"" + refresh_token + "\");");

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

#endif //CREATE_TOKEN_HANDLER_HPP