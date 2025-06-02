#ifndef AUTHORIZE_REQUEST_HANDLER_HPP
#define AUTHORIZE_REQUEST_HANDLER_HPP

#include <Network/HTTPRequests.hpp>
#include <Network/URL.hpp>
#include <fstream>
#include <memory>
#include <string>


namespace handlers
{
    net::HTTPResponse authorize(net::HTTPRequest request)
    {
        net::HTTPResponse response;
        net::URI uri(request.start_line[1]);

        std::cout << "AUTHORIZATION\n";
        std::cout << uri.getParamsPtr()["login"] << std::endl;
        std::cout << uri.getParamsPtr()["password"] << std::endl;

        response.start_line[0] = "HTTP/1.1";
        response.start_line[1] = "200";
        response.start_line[2] = "OK";

        response.headers["Version"] = "HTTP/1.1";
        response.headers["Content-Length"] = std::to_string(response.body.length());

        return response;
    }
}

#endif //AUTHORIZE_REQUEST_HANDLER_HPP