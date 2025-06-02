#ifndef HANDLERS_ADDER_HPP
#define HANDLERS_ADDER_HPP

#include <Network/HTTPServer.hpp>
#include <Network/HTTPRequests.hpp>
#include <functional>
#include <utility>
#include <string>

#include "request_handlers/main_page_handler.hpp"
#include "request_handlers/get_file_handler.hpp"

void addHandlers(net::HTTPServer& server)
{
	std::vector<std::pair<std::string, std::function<net::HTTPResponse(net::HTTPRequest)>>> handlers = {
		std::make_pair("/", main_page),
		std::make_pair("/get_file", get_file)
	};

	for (unsigned int i = 0; i < handlers.size(); i++)
		server.addHandler(handlers[i].first, handlers[i].second);
}

#endif //HANDLERS_ADDER_HPP