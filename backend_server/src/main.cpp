#include <iostream>
#include <string>
#include <utility>

#include "request_handlers/database.hpp"
#include "handlers.hpp"

#include "Network/HTTPServer.hpp"
#include "Network/ServerSessionData.hpp"
#include "Network/Timer.hpp"
#include <windows.h>



int main()
{
    db::exec("SELECT * FROM users;");
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
    addHandlers(server);

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

                server.init(8008, localhost == "1");
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

                //std::cout << "a\n";
                if (GetAsyncKeyState(VK_SPACE) < 0)
                    state = ServerStates::PAUSE;
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