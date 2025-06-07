#ifndef GENERATE_JWT_FUNCTIONS_HPP
#define GENERATE_JWT_FUNCTIONS_HPP

#include <JSTypes/JSTypes.hpp>
#include <utility>
#include <string>
#include <ctime>

#include "jwt.hpp"


std::pair<std::string, std::string> generateJWT(std::string id, unsigned int access_lifetime, unsigned int refresh_lifetime)
{
	jst::JSObject payload;
    payload.addField("id", std::make_shared<jst::JSString>(id));
    payload.addField("exp", std::make_shared<jst::JSNumber>(time(NULL) + access_lifetime));
    std::string access_token = jwt::createToken(payload);

    payload.removeField("exp");
    payload.addField("exp", std::make_shared<jst::JSNumber>(time(NULL) + refresh_lifetime));
    std::string refresh_token = jwt::createToken(payload);

    return std::make_pair(access_token, refresh_token);
}

#endif //GENERATE_JWT_FUNCTIONS_HPP