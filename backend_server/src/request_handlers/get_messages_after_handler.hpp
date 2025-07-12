#ifndef GET_MESSAGES_AFTER_HANDLER_HPP
#define GET_MESSAGES_AFTER_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <Network/Timer.hpp>
#include <unordered_map>
#include <utility>
#include <string>
#include <memory>

#include "tools/database.hpp"
#include "tools/jwt.hpp"


namespace handlers
{
    net::HTTPResponse get_messages_after(net::HTTPRequest request)
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
                std::string chat_id = uri.getParamsPtr()["chat_id"];
                std::string message_id = uri.getParamsPtr()["message_id"];
                std::string count = uri.getParamsPtr()["count"];

                std::string sql_req;
                if (count == "all")
                	sql_req = "SELECT id, user_id, text FROM messages WHERE chat_id = " + chat_id + " AND id > " + message_id + ";";
                else
                	sql_req = "SELECT id, user_id, text FROM messages WHERE chat_id = " + chat_id + " AND id > " + message_id + " ORDER BY id ASC LIMIT " + count + ";";
                
                std::shared_ptr<db::DataBuffer> chat_messages_data;
                while (true)
                {
	                chat_messages_data = db::exec(sql_req);
	                if (chat_messages_data->size() > 0)
                        break;
                    Timer::sleep(500000);
                }

                jst::JSObject json;
                json.addField("messages", std::make_shared<jst::JSArray>());

                for (unsigned int i = 0; i < chat_messages_data->size(); i++)
                {
                    jst::JSObject message;
                    message.addField("id", std::make_shared<jst::JSNumber>(stoi(chat_messages_data->at(i)["id"])));
                    message.addField("user_id", std::make_shared<jst::JSNumber>(stoi(chat_messages_data->at(i)["user_id"])));
                    message.addField("text", std::make_shared<jst::JSString>(chat_messages_data->at(i)["text"]));
                    std::static_pointer_cast<jst::JSArray>(json["messages"])->pushBack(std::make_shared<jst::JSObject>(message));
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


#endif //GET_MESSAGES_AFTER_HANDLER_HPP