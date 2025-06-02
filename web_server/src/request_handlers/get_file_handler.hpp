#ifndef GET_FILE_HANDLER_HPP
#define GET_FILE_HANDLER_HPP

#include <Network/HTTPRequests.hpp>
#include <Network/URL.hpp>

#include <fstream>

std::string get_content_type(std::string path)
{
    if (path.find(".html") != std::string::npos)
        return "text/html";
    if (path.find(".css") != std::string::npos)
        return "text/css";
    if (path.find(".js") != std::string::npos)
        return "application/javascript";
    return "text/plane";
}


net::HTTPResponse get_file(net::HTTPRequest request)
{
	net::HTTPResponse response;
    net::URI uri(request.start_line[1]);
    std::cout << "PATH: " << uri.getParamsPtr()["path"] << std::endl;

    std::unique_ptr<std::string> data_ptr = load_file_data_ptr("resources/" + uri.getParamsPtr()["path"]);
    if (data_ptr == nullptr)
    {
        response.start_line[1] = "400";
        response.start_line[2] = "BAD REQUEST";
        response.headers["Content-Type"] = "text/plane; charset=utf-8";
        response.body = "Incorrect filepath!";
    }
    else
    {
        response.start_line[1] = "200";
        response.start_line[2] = "OK";
        response.headers["Content-Type"] = get_content_type(uri.getParamsPtr()["path"]) + "; charset=utf-8";
        response.body = *data_ptr;
    }


    response.start_line[0] = "HTTP/1.1";

    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Length"] = std::to_string(response.body.length());

    return response;
}

#endif //GET_FILE_HANDLER_HPP