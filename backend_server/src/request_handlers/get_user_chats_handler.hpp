#ifndef GET_USER_CHATS_HANDLER_HPP
#define GET_USER_CHATS_HANDLER_HPP

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
    net::HTTPResponse get_user_chats(net::HTTPRequest request)
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
                std::string user_id = std::static_pointer_cast<jst::JSString>(payload_ptr->operator[]("id"))->getString();
                std::shared_ptr<db::DataBuffer> result = db::exec("SELECT chat_id FROM chat_members WHERE user_id = " + user_id + ";");
                /*
                    chats: [{id, name, type}, {...}]
                */
                jst::JSObject json;
                json.addField("chats", std::make_shared<jst::JSArray>());
                for (unsigned int i = 0; i < result->size(); i++)
                {
                    std::shared_ptr<db::DataBuffer> chat_data = db::exec("SELECT name, type FROM chats WHERE id = " + result->at(i)["chat_id"] + ";");
                    if (chat_data->back()["type"] == "user")
                    {
                        std::shared_ptr<db::DataBuffer> user_data1 = db::exec("SELECT user_id FROM chat_members WHERE chat_id = " + result->at(i)["chat_id"] + ";");
                        std::string id = (user_id == user_data1->at(0)["user_id"]) ? user_data1->at(1)["user_id"] : user_data1->at(0)["user_id"];
                        std::shared_ptr<db::DataBuffer> name_data2 = db::exec("SELECT username FROM users WHERE id = " + id + ";");
                        chat_data->back()["name"] = name_data2->back()["username"];
                    }

                    jst::JSObject chat_obj;
                    chat_obj.addField("id", std::make_shared<jst::JSNumber>(stoi(result->at(i)["chat_id"])));
                    chat_obj.addField("name", std::make_shared<jst::JSString>(chat_data->back()["name"]));
                    chat_obj.addField("type", std::make_shared<jst::JSString>(chat_data->back()["type"]));
                    std::static_pointer_cast<jst::JSArray>(json["chats"])->pushBack(std::make_shared<jst::JSObject>(chat_obj));
                }

                response.start_line[1] = "200";
                response.start_line[2] = "OK";
                response.headers["Content-Type"] = "application/json";
                response.body = json.toString();
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

#endif //GET_USER_CHATS_HANDLER_HPP