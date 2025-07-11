#ifndef GET_CHAT_DATA_HANDLER_HPP
#define GET_CHAT_DATA_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <unordered_map>
#include <fstream>
#include <sstream>
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
                std::string chat_id = uri.getParamsPtr()["id"];
                std::shared_ptr<db::DataBuffer> chat_members_data = db::exec("SELECT chat_members.user_id, users.username, users.has_profile_photo\
                                                                              FROM chat_members\
                                                                              INNER JOIN users ON chat_members.user_id = users.id\
                                                                              WHERE chat_members.chat_id = " + chat_id + ";");
                std::shared_ptr<db::DataBuffer> chat_data = db::exec("SELECT * FROM chats WHERE id = " + chat_id + ";");
                if (chat_data->back()["type"] == "user")
                {
                    std::shared_ptr<db::DataBuffer> user_data = db::exec("SELECT user_id FROM chat_members WHERE chat_id = " + chat_id + ";");
                    std::string id = (user_id == user_data->at(0)["user_id"]) ? user_data->at(1)["user_id"] : user_data->at(0)["user_id"];
                    std::shared_ptr<db::DataBuffer> name_data = db::exec("SELECT username FROM users WHERE id = " + id + ";");
                    chat_data->back()["name"] = name_data->back()["username"];
                }
                
                jst::JSObject json_chat;
                json_chat.addField("type", std::make_shared<jst::JSString>(chat_data->back()["type"]));
                json_chat.addField("name", std::make_shared<jst::JSString>(chat_data->back()["name"]));
                json_chat.addField("members", std::make_shared<jst::JSArray>());

                for (unsigned int i = 0; i < chat_members_data->size(); i++)
                {
                    std::ifstream file;
                    if (chat_members_data->at(i)["has_profile_photo"] == "1")
                        file.open("resources/profile_photos/" + chat_members_data->at(i)["user_id"] + ".png", std::ios::binary);
                    else
                        file.open("resources/profile_photos/default.png", std::ios::binary);
                    std::ostringstream photo_bin;
                    photo_bin << file.rdbuf();
                    file.close();

                    jst::JSObject member;
                    member.addField("id", std::make_shared<jst::JSNumber>(stoi(chat_members_data->at(i)["user_id"])));
                    member.addField("username", std::make_shared<jst::JSString>(chat_members_data->at(i)["username"]));
                    member.addField("profile_photo_64", std::make_shared<jst::JSString>(Base64::encode(photo_bin.str())));
                    std::static_pointer_cast<jst::JSArray>(json_chat["members"])->pushBack(std::make_shared<jst::JSObject>(member));
                }

                response.start_line[1] = "200";
                response.start_line[2] = "OK";
                response.headers["Content-Type"] = "application/json";
                response.body = json_chat.toString();
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

#endif //GET_CHAT_DATA_HANDLER_HPP