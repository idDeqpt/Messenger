#include "APIServer.hpp"

#include <Network/HTTP.hpp>
#include <Network/HTTPRequests.hpp>

#include <unordered_map>
#include <functional>
#include <utility>
#include <string>

#include "request_handlers/tools/sha1.hpp"
#include "request_handlers/tools/base64.hpp"

#include "request_handlers/http_signup_handler.hpp"
#include "request_handlers/http_login_handler.hpp"
#include "request_handlers/http_check_access_token_handler.hpp"
#include "request_handlers/http_update_token_handler.hpp"
#include "request_handlers/http_get_username_handler.hpp"
#include "request_handlers/http_change_username_handler.hpp"
#include "request_handlers/http_get_user_chats_handler.hpp"
#include "request_handlers/http_get_chat_data_handler.hpp"
#include "request_handlers/http_get_chat_messages_handler.hpp"
#include "request_handlers/http_send_message_handler.hpp"
#include "request_handlers/http_add_member_to_chat_handler.hpp"
#include "request_handlers/http_create_chat_handler.hpp"
#include "request_handlers/http_get_profile_data_handler.hpp"
#include "request_handlers/http_change_profile_photo_handler.hpp"
#include "request_handlers/http_change_description_handler.hpp"
#include "request_handlers/http_get_messages_after_handler.hpp"


namespace Handlers
{
	const std::unordered_map<std::string, std::function<net::HTTPResponse(net::HTTPRequest)>> http = {
		{"/signup",               handlers::http::signup},
		{"/login",                handlers::http::login},
		{"/check_access_token",   handlers::http::check_access_token},
		{"/update_token",         handlers::http::update_token},
		{"/get_username",         handlers::http::get_username},
		{"/change_username",      handlers::http::change_username},
		{"/get_user_chats",       handlers::http::get_user_chats},
		{"/get_chat_data",        handlers::http::get_chat_data},
		{"/get_chat_messages",    handlers::http::get_chat_messages},
		{"/send_message",         handlers::http::send_message},
		{"/add_member_to_chat",   handlers::http::add_member_to_chat},
		{"/create_chat",          handlers::http::create_chat},
		{"/get_profile_data",     handlers::http::get_profile_data},
		{"/change_profile_photo", handlers::http::change_profile_photo},
		{"/change_description",   handlers::http::change_description},
		{"/get_messages_after",   handlers::http::get_messages_after}
	};

	const std::unordered_map<std::string, std::function<void(net::TCPServer*, int)>> ws = {};
}


namespace app
{

template <class Base>
APIServerMixin<Base>::APIServerMixin(const std::string& first) : Base(first) {}

template <class Base>
APIServerMixin<Base>::APIServerMixin(const std::string& first, const std::string& second) : Base(first, second) {}


template <class Base>
void APIServerMixin<Base>::connection_handler(int client_socket)
{
	std::string raw_request = this->recv(client_socket);
	if (raw_request.empty()) return;

	net::HTTPRequest request(raw_request);
	session_handler(request, client_socket);
}

template <class Base>
void APIServerMixin<Base>::session_handler(net::HTTPRequest request, int client_socket)
{
	net::URI uri(request.start_line[1]);
	std::string path = uri.toString(false);

	auto iter_http = Handlers::http.find(path);
	if (iter_http != Handlers::http.end())
	{
		net::HTTPResponse response = iter_http->second(request);
		response.headers["Connection"] = "keep-alive";
		std::string response_str = response.toString();
		this->send(client_socket, response_str);
		return;
	}

	net::HTTPResponse response;
    response.body = "<p>404</p>";
    response.start_line[0] = "HTTP/1.1";
    response.start_line[1] = "404";
    response.start_line[2] = "NOT FOUND";
    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Type"] = "text/html; charset=utf-8";
    response.headers["Version"] = "HTTP/1.1";
	response.headers["Connection"] = "keep-alive";
    response.headers["Content-Length"] = std::to_string(response.body.length());
	this->send(client_socket, response.toString());
}

} //namespace app