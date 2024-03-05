#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP
# include "Request.hpp"
# include "Response.hpp"
# include "ServerConfig.hpp"

class ClientInfo
{
	public:
		int id;
		Request request;
		Response response;
		ServerInfo serverInfo;
		//std::string requestBuffer;
		//std::string rem;
		//std::string responseBuffer;
		//std::vector<std::string> lines;
		//size_t linesIdx;
		ClientInfo();
		ClientInfo( int id, const ServerInfo & serverInfo );
		ClientInfo( const ClientInfo & clientInfo);
		ClientInfo & operator=( const ClientInfo & clientInfo);
		~ClientInfo();
		void setup( const std::string & str );
		//void request_buffer_prep();
		//bool reach_end();
		//std::string get_response();
};

#endif
