#include "webserv.hpp"

ServerConfig::ServerConfig()
{

}


ServerConfig::ServerConfig( const ServerConfig & serverConfig )
{
	*this = serverConfig;
}

ServerConfig & ServerConfig::operator=( const ServerConfig & serverConfig )
{
	if ( this != &serverConfig )
	{
		this->server_name = serverConfig.server_name;
		this->error_pages = serverConfig.error_pages;
		this->client_body_size = serverConfig.client_body_size;
		this->location = serverConfig.location;
	}
	return (*this);
}

ServerConfig::~ServerConfig()
{

}
