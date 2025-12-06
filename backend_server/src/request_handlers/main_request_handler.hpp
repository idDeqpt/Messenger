#ifndef HANDLERS_ADDER_HPP
#define HANDLERS_ADDER_HPP

#include <Network/HTTPRequests.hpp>
#include <Network/TCPServer.hpp>
#include <unordered_map>
#include <functional>
#include <utility>
#include <string>

#include "tools/sha1.hpp"
#include "tools/base64.hpp"

#include "http_signup_handler.hpp"
#include "http_login_handler.hpp"
#include "http_check_access_token_handler.hpp"
#include "http_update_token_handler.hpp"
#include "http_get_username_handler.hpp"
#include "http_change_username_handler.hpp"
#include "http_get_user_chats_handler.hpp"
#include "http_get_chat_data_handler.hpp"
#include "http_get_chat_messages_handler.hpp"
#include "http_send_message_handler.hpp"
#include "http_add_member_to_chat_handler.hpp"
#include "http_create_chat_handler.hpp"
#include "http_get_profile_data_handler.hpp"
#include "http_change_profile_photo_handler.hpp"
#include "http_change_description_handler.hpp"
#include "http_get_messages_after_handler.hpp"


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


void request_handler(net::TCPServer* server, int client_socket)
{
	net::HTTPRequest request(server->recv(client_socket));
	net::URI uri(request.start_line[1]);
	std::string path = uri.toString(false);

	if ((request.headers["Connection"] == "Upgrade") && (request.headers["Upgrade"] == "websocket"))
	{
		auto iter_ws = Handlers::ws.find(path);
		if (iter_ws != Handlers::ws.end())
		{
			SHA1 sha;
			sha.update(request.headers["Sec-WebSocket-Key"] + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
			std::string sha1_hash_hex = sha.final();
			std::string binary_data = "";
			for (unsigned int i = 0; i < sha1_hash_hex.length(); i += 2)
			{
				std::string byteString = sha1_hash_hex.substr(i, 2);
				unsigned char byteValue = (unsigned char) strtol(byteString.c_str(), nullptr, 16);
				binary_data += byteValue;
			}
			
			net::HTTPResponse response;
			response.start_line[0] = "HTTP/1.1";
			response.start_line[1] = "101";
			response.start_line[2] = "Switching Protocols";
			response.headers["Upgrade"] = "websocket";
			response.headers["Connection"] = "Upgrade";
			response.headers["Sec-WebSocket-Accept"] = Base64::encode(binary_data);
			server->send(client_socket, response.toString());
			iter_ws->second(server, client_socket);
			return;
		}
	}
	else
	{
		auto iter_http = Handlers::http.find(path);
		if (iter_http != Handlers::http.end())
		{
			std::string response_str = iter_http->second(request).toString();
			server->send(client_socket, response_str);
			return;
		}
	}

	net::HTTPResponse response;
    response.body = "<p>404</p>";
    response.start_line[0] = "HTTP/1.1";
    response.start_line[1] = "404";
    response.start_line[2] = "NOT FOUND";
    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Type"] = "text/html; charset=utf-8";
    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Length"] = std::to_string(response.body.length());
	server->send(client_socket, response.toString());
}

#endif //HANDLERS_ADDER_HPP