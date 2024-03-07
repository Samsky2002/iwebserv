#include "webserv.hpp"

Server::Server()
{

}

Server & Server::operator=( const Server & server )
{
	if ( this != &server )
	{
		this->serverInfo = server.serverInfo;
	}
	return ( *this );
}

Server::~Server()
{

}
// launch function

void Server::execute( const std::string & file )
{
	setup( file );
	launch();
}

void Server::setup( const std::string & file )
{
	(void)file;
	// parse and stuff
	ServerInfo serverInfo1;
	serverInfo1.port = 800;
	serverInfo1.host = "127.0.0.1";

	ServerInfo serverInfo2;
	serverInfo2.port = 801;
	serverInfo2.host = "127.0.0.1";

	ServerConfig serverConfig1;
	serverConfig1.server_name = "oussama1";
	//serverConfig1.error_pages.push_back( std::make_pair( 404, "/Users/oakerkao/Desktop/nginx/html/404.html" ) );

	ServerConfig serverConfig2;
	serverConfig2.server_name = "oussama2";

	Location location1;
	location1.path = "/";
	location1.root = "/Users/oakerkao/Desktop/nginx/html/";
	location1.methods.push_back("GET");
	location1.index.push_back("inde.html");

	Location location2;
	location2.path = "/cgi-bin/";
	location2.root = "/Users/oakerkao/Desktop/nginx/html/cgi-bin/";
	//location2.index.push_back("script.py");
	location2.autoindex = true;
	location2.methods.push_back("POST");

	serverConfig1.location.push_back( location1 );
	serverConfig1.location.push_back( location2 );
	serverConfig2.location.push_back( location1 );
	serverInfo1.serverConfig.push_back( serverConfig1 );
	serverInfo1.serverConfig.push_back( serverConfig2 );
	serverInfo.push_back( serverInfo1 );
	serverInfo.push_back( serverInfo2 );
}

void Server::launch()
{
	for ( size_t i = 0; i < serverInfo.size(); i++ )
	{
		serverInfo[i].launch();
	}
}

ServerInfo Server::getServer( int id ) const
{
	size_t i = 0;

	while ( i < serverInfo.size() )
	{
		if ( id == serverInfo[i].id)
			break ;
		i++;
	}
	return ( serverInfo[i] );
}
