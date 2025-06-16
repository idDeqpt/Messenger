#ifndef SEND_MESSAGE_HANDLER_HPP
#define SEND_MESSAGE_HANDLER_HPP

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
    net::HTTPResponse send_message(net::HTTPRequest request)
    {
        net::HTTPResponse response;
        net::URI uri(request.start_line[1]);

    	if (request.start_line[0] == "POST")
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
                std::string user_id = std::static_pointer_cast<jst::JSString>(payload_ptr->operator[]("id"))->getString();
                
                jst::JSON parser;
                parser.parse(request.body);
                std::shared_ptr<jst::JSObject> request_data = std::static_pointer_cast<jst::JSObject>(parser.getParseResult());
                
                db::exec("INSERT INTO messages (chat_id, user_id, text)\
                          VALUES (" + request_data->operator[]("chat_id")->toString() + ", " + 
                                      user_id + ", " + 
                                      request_data->operator[]("text")->toString() + ");");
                
                response.start_line[1] = "200";
                response.start_line[2] = "OK";
            }
        }
        else if (request.start_line[0] == "OPTIONS")
        {
            response.start_line[1] = "200";
            response.start_line[2] = "OK";
            response.headers["Access-Control-Allow-Methods"] = "OPTIONS,POST";
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

#endif //SEND_MESSAGE_HANDLER_HPP