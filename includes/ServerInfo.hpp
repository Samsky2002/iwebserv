#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP
# include "ServerConfig.hpp"

class ServerInfo
{
	public:
		int id;
		int port;
		std::string host;
		std::vector< ServerConfig > serverConfig;
		ServerInfo();
		ServerInfo( const ServerInfo & serverInfo );
		ServerInfo & operator=( const ServerInfo & serverInfo );
		~ServerInfo();
		void launch();
};

#endif
