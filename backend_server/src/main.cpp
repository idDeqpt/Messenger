#include <iostream>
#include <utility>
#include <string>

#include <Network/TCPServer.hpp>
#include <Network/ServerSessionData.hpp>
#include <Network/Timer.hpp>

#ifdef _WIN32
	#define WIN(exp) exp
	#define NIX(exp)
#else
	#define WIN(exp)
	#define NIX(exp) exp
#endif

#include "request_handlers/main_request_handler.hpp"
#include "request_handlers/tools/database.hpp"


int main(int argc, char* argv[])
{
	std::string path = argv[0];
	int last_slash = path.rfind(
		WIN("\\")
		NIX("/")
	);
	db::work_directory = path.substr(0, last_slash + 1);

	int port = -1;
	if (argc > 1)
		try
		{
			port = std::stoi(std::string(argv[1]));
			std::cout << "Port: " << port << std::endl;
		} catch(...) {}

	while (port < 0)
	{
		std::cout << "Enter the port: ";
		std::string port_s;
		std::cin >> port_s;
		try
		{
			port = std::stoi(port_s);
		}
		catch(...)
		{
			std::cout << "Unexpected character!" << std::endl;
		}
	}

	net::TCPServer server;
	server.setRequestHandler(request_handler);

	int init_status = server.init(port);
	if (!server.start())
	{
		std::cout << "Server start incompleted with code " << init_status << std::endl;

		system("pause");
		return 0;
	}
	std::cout << "Server start completed\n------------------------------------------------------------------------------------------\n";

	Timer timer;
	while (true)
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