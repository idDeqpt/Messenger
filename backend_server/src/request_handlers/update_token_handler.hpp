#ifndef UPDATE_TOKEN_HANDLER_HPP
#define UPDATE_TOKEN_HANDLER_HPP

#include <JSTypes/JSTypes.hpp>
#include <Network/HTTP.hpp>
#include <utility>
#include <string>

#include "tools/generateJWT.hpp"
#include "tools/database.hpp"
#include "tools/jwt.hpp"


namespace handlers
{
    net::HTTPResponse update_token(net::HTTPRequest request)
    {
        net::HTTPResponse response;
        bool unauthorized = false;

        if (request.start_line[0] == "POST")
        {
        	std::string token = request.headers["Authorization"];
        	if (jwt::verifyToken(token) != jwt::TokenError::NO_ERROR)
        		unauthorized = true;
        	else
        	{
        		std::shared_ptr<jst::JSObject> payload_ptr = jwt::getPayload(token);
        		std::string id = std::static_pointer_cast<jst::JSString>(payload_ptr->operator[]("id"))->getString();
        		std::shared_ptr<db::DataBuffer> result = db::exec("SELECT refresh_token FROM users WHERE id = \"" + id + "\";");
                if (result->size() == 0)
                    unauthorized = true;
                else
                {
            		std::string db_token = result->back()["refresh_token"];
                    if (token != db_token)
            			unauthorized = true;
            		else
            		{
            			std::pair<std::string, std::string> tokens = generateJWT(id, 60*60, 60*60*24*3);
            			db::exec("INSERT INTO users (refresh_token)\
                              	  VALUES (\"" + tokens.second + "\");");
            			jst::JSObject json;
    	                json.addField("access_token", std::make_shared<jst::JSString>(tokens.first));
    	                json.addField("refresh_token", std::make_shared<jst::JSString>(tokens.second));

    	                response.start_line[1] = "200";
    	                response.start_line[2] = "OK";
    	                response.body = json.toString();
            		}
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

        if (unauthorized)
        {
            response.start_line[1] = "401";
            response.start_line[2] = "UNAUTHORIZED";
        }

        response.start_line[0] = "HTTP/1.1";
        response.headers["Version"] = "HTTP/1.1";
        response.headers["Access-Control-Allow-Origin"] = "*";
        response.headers["Content-Length"] = std::to_string(response.body.length());

        return response;
    }
}

#endif //UPDATE_TOKEN_HANDLER_HPP