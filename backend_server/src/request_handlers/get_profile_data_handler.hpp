#ifndef GET_PROFILE_DATA_HANDLER_HPP
#define GET_PROFILE_DATA_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <memory>

#include "tools/database.hpp"
#include "tools/base64.hpp"
#include "tools/jwt.hpp"


namespace handlers
{
    net::HTTPResponse get_profile_data(net::HTTPRequest request)
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
                std::string user_id = uri.getParamsPtr()["id"];
                std::shared_ptr<db::DataBuffer> user_data = db::exec("SELECT username, description, has_profile_photo FROM users WHERE id = " + user_id + ";");
                
                std::ifstream file;
                if (user_data->back()["has_profile_photo"] == "1")
                    file.open("resources/profile_photos/" + user_id + ".png", std::ios::binary);
                else
                    file.open("resources/profile_photos/default.png", std::ios::binary);

                std::ostringstream oss;
                oss << file.rdbuf();
                file.close();

                jst::JSObject user_json;
                user_json.addField("username", std::make_shared<jst::JSString>(user_data->back()["username"]));
                user_json.addField("description", std::make_shared<jst::JSString>(user_data->back()["description"]));
                user_json.addField("profile_photo_64", std::make_shared<jst::JSString>(Base64::encode(oss.str())));

                response.start_line[1] = "200";
                response.start_line[2] = "OK";
                response.headers["Content-Type"] = "application/json";
                response.body = user_json.toString();
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

#endif //GET_PROFILE_DATA_HANDLER_HPP