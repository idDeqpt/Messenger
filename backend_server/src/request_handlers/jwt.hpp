#ifndef JWT_FUNCTIONALITY_HPP
#define JWT_FUNCTIONALITY_HPP

#include <JSTypes/JSTypes.hpp>
#include <string>
#include <memory>
#include <ctime>

#include "base64.hpp"
#include "sha256.hpp"


namespace jwt
{
	std::string secret_key = "secret";
	enum TokenError
	{
		NO_ERROR,
		WRONG_TOKEN,
		INVALID_SIGNATURE,
		TOKEN_EXPIRED,
	};

	std::string createSignature(std::string& header, std::string& payload, std::string& key)
	{
		return SHA256::hash(header + "." + payload + "." + key);
	}

	std::string createToken(jst::JSObject& payload)
	{
		jst::JSObject header;
		header.addField("alg", std::make_shared<jst::JSString>("HS256"));
		header.addField("typ", std::make_shared<jst::JSString>("JWT"));

		std::string encodedHeader = Base64::encode(header.toString());
		std::string encodedPayload = Base64::encode(payload.toString());
		std::string signature = Base64::encode(createSignature(encodedHeader, encodedPayload, secret_key));

		return encodedHeader + "." + encodedPayload + "." + signature;
	}

	TokenError verifyToken(std::string token)
	{
		int left_point = token.find(".");
		if (left_point == std::string::npos)
			return TokenError::WRONG_TOKEN;
		std::string header = token.substr(0, left_point);

		int right_point = token.find(".", left_point + 1);
		if (right_point == std::string::npos)
			return TokenError::WRONG_TOKEN;
		std::string payload = token.substr(left_point + 1, right_point - left_point - 1);

		std::string signature = token.substr(right_point + 1, token.size() - right_point - 1);
		std::string real_signature = Base64::encode(createSignature(header, payload, secret_key));

		if (signature != real_signature)
			return TokenError::INVALID_SIGNATURE;

		jst::JSON json;
		json.parse(Base64::decode(payload));
		std::shared_ptr<jst::JSObject> payload_ptr = std::static_pointer_cast<jst::JSObject>(json.getParseResult());

		time_t cur_time;
		time (&cur_time);
		unsigned int jwt_time = 0;
		try
		{
			jwt_time = stoi(payload_ptr->operator[]("exp")->toString());
		} catch(...) {}
		std::cout << jwt_time << " " << cur_time << std::endl;
		if ((payload_ptr->operator[]("exp") == nullptr) || (jwt_time > cur_time))
			return TokenError::NO_ERROR;

		return TokenError::TOKEN_EXPIRED;
	}
}

#endif //JWT_FUNCTIONALITY_HPP