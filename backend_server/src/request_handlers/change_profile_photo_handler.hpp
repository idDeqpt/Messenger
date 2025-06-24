#ifndef CHANGE_PROFILE_PHOTO_HANDLER_HPP
#define CHANGE_PROFILE_PHOTO_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <string>
#include <memory>

#include "tools/database.hpp"
#include "tools/base64.hpp"
#include "tools/jwt.hpp"


namespace handlers
{
    net::HTTPResponse change_profile_photo(net::HTTPRequest request)
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
                
                std::ofstream file("resources/profile_photos/" + user_id + ".png", std::ios::binary);
                file << Base64::decode(std::static_pointer_cast<jst::JSString>(request_data->operator[]("photo_64"))->getString());
                file.close();
                db::exec("UPDATE users SET has_profile_photo = 1 WHERE id = " + user_id);

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

#endif //CHANGE_PROFILE_PHOTO_HANDLER_HPP