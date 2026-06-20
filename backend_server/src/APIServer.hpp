#ifndef APP_API_SERVER_CLASS_HEADER
#define APP_API_SERVER_CLASS_HEADER

#include <Network/HTTP.hpp>
#include <Network/WebServer.hpp>
#include <Network/WebServerSecure.hpp>
#include <string>

namespace app
{
	template <typename Base>
	class APIServerMixin : public Base
	{
	public:
		APIServerMixin(const std::string&);
		APIServerMixin(const std::string&, const std::string&);
		
	protected:
		void connection_handler(int client_socket) override;
		void session_handler(net::HTTPRequest request, int client_socket) override;
	};

	using APIServer       = APIServerMixin<net::WebServer>;
	using APIServerSecure = APIServerMixin<net::WebServerSecure>;
}

#include "APIServer.inl"

#endif //APP_API_SERVER_CLASS_HEADER