#ifndef HANDLERS_ADDER_HPP
#define HANDLERS_ADDER_HPP

#include <Network/HTTPServer.hpp>
#include <Network/HTTPRequests.hpp>
#include <functional>
#include <utility>
#include <string>

#include "request_handlers/authorize_handler.hpp"

void addHandlers(net::HTTPServer& server)
{
	std::vector<std::pair<std::string, std::function<net::HTTPResponse(net::HTTPRequest)>>> handlers = {
		std::make_pair("/authorize", handlers::authorize)
	};

	for (unsigned int i = 0; i < handlers.size(); i++)
		server.addHandler(handlers[i].first, handlers[i].second);
}

#endif //HANDLERS_ADDER_HPP