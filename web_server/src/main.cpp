#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include <functional>
#include <unordered_map>
#include <utility>

#include "Network/URL.hpp"
#include "Network/HTTP.hpp"
#include "Network/HTTPServer.hpp"
#include "Network/ServerSessionData.hpp"
#include "Network/Timer.hpp"


#ifdef _WIN32
    #define WIN(exp) exp
    #define NIX(exp)
#else
    #define WIN(exp)
    #define NIX(exp) exp
#endif


std::string work_directory = "";


std::string get_content_type(std::string path)
{
	if (path.find(".html") != std::string::npos)
		return "text/html";
	if (path.find(".css") != std::string::npos)
		return "text/css";
	if (path.find(".js") != std::string::npos)
		return "application/javascript";
	if (path.find(".png") != std::string::npos)
		return "image/png";
	return "text/plane";
}


std::unique_ptr<std::string> load_file_data_ptr(std::string path)
{
	std::ifstream file(path, std::ios::binary);

	if (!file)
		return std::unique_ptr<std::string>(nullptr);

	std::ostringstream oss;
	oss << file.rdbuf();
	file.close();
	return std::make_unique<std::string>(oss.str());
}


void http_handler(net::TCPServer* server, int client_socket)
{
	net::HTTPServer* http_server = dynamic_cast<net::HTTPServer*>(server);
	net::HTTPResponse response;
	net::HTTPRequest req(server->recv(client_socket));
	net::URI uri(req.start_line[1]);
	std::string path = uri.toString(false);

	if (path.find(".") == std::string::npos)
		path += "/index.html";

	std::unique_ptr<std::string> data_ptr = load_file_data_ptr(work_directory + "resources" + path);
	if (data_ptr == nullptr)
	{
		server->send(client_socket, http_server->get404Handler()().toString());
		return;
	}

	response.body = *data_ptr;
	response.start_line[0] = "HTTP/1.1";
	response.start_line[1] = "200";
	response.start_line[2] = "OK";
	response.headers["Version"] = "HTTP/1.1";
	response.headers["Content-Type"] = get_content_type(path) + "; charset=utf-8";
	response.headers["Content-Length"] = std::to_string(response.body.length());

	server->send(client_socket, response.toString());
}


int main(int argc, char* argv[])
{
	std::string path = argv[0];
	int last_slash = path.rfind(
		WIN("\\")
		NIX("/")
	);
	work_directory = path.substr(0, last_slash + 1);

	int port = -1;
	if (argc > 1)
		try
		{
			port = std::stoi(std::string(argv[1]));
			std::cout << "Port: " << port << std::endl;
		} catch (...) {}

	while (port < 0)
	{
		std::cout << "Enter the port: ";
		std::string port_s;
		std::cin >> port_s;
		try
		{
			port = std::stoi(port_s);
		}
		catch (...)
		{
			std::cout << "Unexpected character!" << std::endl;
		}
	}

	net::HTTPServer server;
	server.setRequestHandler(http_handler);
	int init_status = server.init(port);

	if (!server.start())
	{
		std::cout << "Server start incompleted: " << init_status << std::endl;
		return 0;
	}
	std::cout << "Server start completed\n------------------------------------------------------------------------------------------\n";

	Timer timer;
	while(true)
	{
		if (server.hasNewSessionData())
		{
			static constexpr char* session_data_types[] = {"Open", "Close", "RECV", "Send"};
			net::ServerSessionData session_data = server.getNextSessionData();
			std::cout << session_data_types[session_data.getType()] << " " << session_data.getId() << " " << session_data.getTime() << "s:" << std::endl
			<< session_data.getText() << std::endl
			<< "==========================================================================================\n";
		}
		timer.sleep(16);
	}

	system("pause");
	return 0;
}