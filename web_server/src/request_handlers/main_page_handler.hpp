#ifndef MAIN_PAGE_HANDLER_HPP
#define MAIN_PAGE_HANDLER_HPP

#include <Network/HTTPRequests.hpp>
#include <Network/URL.hpp>
#include <fstream>
#include <memory>
#include <string>

#include "load_file_data.hpp"


net::HTTPResponse main_page(net::HTTPRequest request)
{
    net::HTTPResponse response;

    std::unique_ptr<std::string> data_ptr = load_file_data_ptr("resources/html/main_page.html");
    if (data_ptr == nullptr)
        response.headers["Content-Type"] = "text/plane; charset=utf-8";
    else
    {
        response.body = *data_ptr;
        response.headers["Content-Type"] = "text/html; charset=utf-8";

    }

    response.start_line[0] = "HTTP/1.1";
    response.start_line[1] = "200";
    response.start_line[2] = "OK";

    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Length"] = std::to_string(response.body.length());

    return response;
}

#endif //MAIN_PAGE_HANDLER_HPP