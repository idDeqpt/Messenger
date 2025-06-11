#ifndef HANDLERS_ADDER_HPP
#define HANDLERS_ADDER_HPP

#include <Network/HTTPRequests.hpp>
#include <Network/HTTPServer.hpp>
#include <functional>
#include <utility>
#include <string>

#include "request_handlers/signup_handler.hpp"
#include "request_handlers/login_handler.hpp"
#include "request_handlers/check_access_token_handler.hpp"
#include "request_handlers/update_token_handler.hpp"
#include "request_handlers/get_username_handler.hpp"
#include "request_handlers/change_username_handler.hpp"
#include "request_handlers/get_user_chats_handler.hpp"
#include "request_handlers/get_chat_data_handler.hpp"


void addHandlers(net::HTTPServer& server)
{
	std::vector<std::pair<std::string, std::function<net::HTTPResponse(net::HTTPRequest)>>> handlers = {
		std::make_pair("/signup", handlers::signup),
		std::make_pair("/login", handlers::login),
		std::make_pair("/check_access_token", handlers::check_access_token),
		std::make_pair("/update_token", handlers::update_token),
		std::make_pair("/get_username", handlers::get_username),
		std::make_pair("/change_username", handlers::change_username),
		std::make_pair("/get_user_chats", handlers::get_user_chats),
		std::make_pair("/get_chat_data", handlers::get_chat_data)
	};

	for (unsigned int i = 0; i < handlers.size(); i++)
		server.addHandler(handlers[i].first, handlers[i].second);
}

#endif //HANDLERS_ADDER_HPP