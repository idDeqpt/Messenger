#ifndef CHECK_ACCESS_TOKEN_HANDLER_HPP
#define CHECK_ACCESS_TOKEN_HANDLER_HPP

#include <Network/HTTP.hpp>
#include <string>

#include "jwt.hpp"


namespace handlers
{
    net::HTTPResponse check_access_token(net::HTTPRequest request)
    {
        net::HTTPResponse response;

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
                response.start_line[1] = "200";
                response.start_line[2] = "OK";
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

#endif //CHECK_ACCESS_TOKEN_HANDLER_HPP