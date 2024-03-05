#include "webserv.hpp"

ServerInfo::ServerInfo()
{
	
}

ServerInfo::ServerInfo( const ServerInfo & serverInfo )
{
	*this = serverInfo;
}

ServerInfo & ServerInfo::operator=( const ServerInfo & serverInfo )
{
	if ( this != &serverInfo )
	{
		this->id = serverInfo.id;
		this->host = serverInfo.host;
		this->port = serverInfo.port;
		this->serverConfig = serverInfo.serverConfig;
	}
	return ( *this );
}

ServerInfo::~ServerInfo()
{

}

void ServerInfo::launch()
{
	SocketServer socketServer( port, host );

	socketServer.launch( id );
}

// you can konw by the id of the serverInfo if it's a socket or not
