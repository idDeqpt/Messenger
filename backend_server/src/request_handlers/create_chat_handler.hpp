#ifndef CREATE_CHAT_HANDLER_HPP
#define CREATE_CHAT_HANDLER_HPP

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
    net::HTTPResponse create_chat(net::HTTPRequest request)
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
                std::string chat_type = std::static_pointer_cast<jst::JSString>(request_data->operator[]("type"))->getString();
                std::string chat_name = std::static_pointer_cast<jst::JSString>(request_data->operator[]("name"))->getString();

                std::shared_ptr<db::DataBuffer> result_not_found = db::exec("SELECT id FROM users WHERE username = \"" + chat_name + "\";");
                if ((chat_type == "user") && (result_not_found->size() == 0))
                {
                    response.start_line[1] = "404";
                    response.start_line[2] = "NOT FOUND";
                }
                else
                {
                    std::shared_ptr<db::DataBuffer> result = db::exec("INSERT INTO chats (name, type)\
                                                                       VALUES (\"" + chat_name + "\", \"" + chat_type + "\")\
                                                                       RETURNING id;");
                    db::exec("INSERT INTO chat_members (chat_id, user_id)\
                              VALUES (" + result->back()["id"] + ", " + user_id + ");");
                    if (chat_type == "user")
                    {
                        db::exec("INSERT INTO chat_members (chat_id, user_id)\
                                  VALUES (" + result->back()["id"] + ", " + result_not_found->back()["id"] + ");");
                    }
                    
                    jst::JSObject json;
                    json.addField("chat_id", std::make_shared<jst::JSNumber>(stoi(result->back()["id"])));

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

#endif //CREATE_CHAT_HANDLER_HPP