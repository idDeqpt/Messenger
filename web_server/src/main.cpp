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
#include "Network/HTTPRequests.hpp"
#include "Network/TCPServer.hpp"
#include "Network/TCPClient.hpp"
#include "Network/ServerSessionData.hpp"
#include "Network/Timer.hpp"
#include <windows.h>


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


std::string http_handler(net::HTTPServer& server, std::string request)
{
    net::HTTPResponse response;
    net::HTTPRequest req(request);
    net::URI uri(req.start_line[1]);
    std::string path = uri.toString(false);

    if (path.find(".") == std::string::npos)
        path += "/index.html";

    std::unique_ptr<std::string> data_ptr = load_file_data_ptr("resources" + path);
    if (data_ptr == nullptr)
        return server.get404Handler()().toString();

    response.body = *data_ptr;
    response.start_line[0] = "HTTP/1.1";
    response.start_line[1] = "200";
    response.start_line[2] = "OK";
    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Type"] = get_content_type(path) + "; charset=utf-8";
    response.headers["Content-Length"] = std::to_string(response.body.length());

    return response.toString();
}


int main()
{
    enum ServerStates
    {
        CHOOSE_STATE = 0,
        INIT,
        EXIT,
        PROCESS,
        PAUSE,
    } state = ServerStates::CHOOSE_STATE;

    std::vector<std::pair<ServerStates, std::string>> menu_selection_variants = {
        {ServerStates::INIT, "Start"},
        {ServerStates::EXIT, "Exit"}
    };

    net::HTTPServer server;
    server.setHTTPHandler(http_handler);

    Timer timer;
    while (state != ServerStates::EXIT)
    {
        switch(state)
        {
            case ServerStates::CHOOSE_STATE:
            {
                std::cout << "Choise action:\n";
                for (unsigned int i = 0; i < menu_selection_variants.size(); i++)
                    std::cout << "\t" << i + 1 << ". " + menu_selection_variants[i].second + ";\n";
                std::cout << "Enter: ";

                std::string action_str;
                std::cin >> action_str;
                std::cout << std::endl;

                try
                {
                    int action = stoi(action_str) - 1;
                    if ((action >= 0) && (action < menu_selection_variants.size()))
                        state = menu_selection_variants[action].first;
                    else
                        std::cout << "Incorrect value!\n";
                }
                catch(...)
                {
                    std::cout << "Incorrect value!\n";
                }
            } break;

            case ServerStates::INIT:
            {
                std::string localhost;
                std::cout << "Start on localhost (\"1\" for accept): ";
                std::cin >> localhost;

                server.init(80, localhost == "1");
                server.start(1);

                std::cout << "Server address: " << server.getSelfAddress().toString() << std::endl;

                state = ServerStates::PROCESS;
            } break;

            case ServerStates::EXIT:
            {
                std::cout << "exit\n";
            } break;

            case ServerStates::PROCESS:
            {
                if (server.hasNewSessionData())
                {
                    net::ServerSessionData session = server.getNextSessionData();
                    std::cout << "Request index: " << session.getId() << "\n"
                    << "REQUEST:\n"
                    << session.getRequest() << "\n"
                    << "RESPONSE:\n"
                    << session.getResponse() << "\n"
                    << "============================\n"
                    << "Press Space for stop...\n\n";
                }
                //if (GetAsyncKeyState(VK_SPACE) < 0)
                    //state = ServerStates::PAUSE;
                timer.sleep(16);
            } break;

            case ServerStates::PAUSE:
            {
                server.stop();
                std::string command;
                std::cout << "For continue enter \"1\", для выхода любое другое значение: ";
                std::cin >> command;

                if (command == "1")
                {
                    server.start(1);
                    state = ServerStates::PROCESS;
                }
                else
                    state = ServerStates::EXIT;
            } break;

            default:
            {
                state = ServerStates::CHOOSE_STATE;
            } break;
        }
    }

	system("pause");
	return 0;
}