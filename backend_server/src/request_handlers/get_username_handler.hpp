#ifndef GET_USERNAME_HANDLER_HPP
#define GET_USERNAME_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <unordered_map>
#include <utility>
#include <string>
#include <memory>

#include "tools/database.hpp"
#include "tools/jwt.hpp"


namespace handlers
{
    net::HTTPResponse get_username(net::HTTPRequest request)
    {
        net::HTTPResponse response;
        net::URI uri(request.start_line[1]);
        bool unauthorized = false;

    	if (request.start_line[0] == "GET")
        {

            std::string token = request.headers["Authorization"];
            if (jwt::verifyToken(token) != jwt::TokenError::NO_ERROR)
            {
                response.start_line[1] = "401";
                response.start_line[2] = "UNAUTHORIZED";
            }
            else
            {
                std::shared_ptr<jst::JSObject> payload_ptr = jwt::getPayload(token);
                std::string id = std::static_pointer_cast<jst::JSString>(payload_ptr->operator[]("id"))->getString();
                std::shared_ptr<db::DataBuffer> result = db::exec("SELECT username FROM users WHERE id = " + id + ";");
                
                if (result->size() != 1)
                {
                    response.start_line[1] = "409";
                    response.start_line[2] = "CONFLICT";
                }
                else
                {
                    jst::JSObject json;
                    json.addField("username", std::make_shared<jst::JSString>(result->back()["username"]));

                    response.start_line[1] = "200";
                    response.start_line[2] = "OK";
                    response.body = json.toString();
                }
            }
        }
        else if (request.start_line[0] == "OPTIONS")
        {
            response.start_line[1] = "200";
            response.start_line[2] = "OK";
            response.headers["Access-Control-Allow-Methods"] = "OPTIONS,GET";
            response.headers["Access-Control-Allow-Headers"] = "Authorization";
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

#endif //GET_USERNAME_HANDLER_HPP