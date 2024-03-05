#ifndef SERVER_HPP
# define SERVER_HPP
# include "ServerInfo.hpp"

class Server
{
	public:
		std::vector<ServerInfo> serverInfo;
		Server();
		Server & operator=(const Server & server);
		~Server();
		const ServerInfo & getServer( int id ) const;
		void execute( const std::string & file );
		void setup( const std::string & file );
		void launch();
};

#endif
