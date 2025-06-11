#ifndef GET_CHAT_DATA_HANDLER_HPP
#define GET_CHAT_DATA_HANDLER_HPP

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
    net::HTTPResponse get_chat_data(net::HTTPRequest request)
    {
        net::HTTPResponse response;
        net::URI uri(request.start_line[1]);
        bool unauthorized = false;

    	if (request.start_line[0] == "GET")
        {
            std::string chat_id = uri.getParamsPtr()["id"];
            std::shared_ptr<db::DataBuffer> result = db::exec("SELECT * FROM chats WHERE id = " + chat_id + ";");
            
            jst::JSArray json;
            for (unsigned int i = 0; i < result->size(); i++)
            {
                jst::JSObject obj;
                for (auto& [key, value] : result->at(i))
                    obj.addField(key, std::make_shared<jst::JSString>(value));
                json.pushBack(std::make_shared<jst::JSObject>(obj));
            }

            response.start_line[1] = "200";
            response.start_line[2] = "OK";
            response.headers["Content-Type"] = "application/json";
            response.body = json.toString();
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

#endif //GET_CHAT_DATA_HANDLER_HPP