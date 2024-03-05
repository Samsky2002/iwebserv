#ifndef SERVERCONFIG_HPP 
# define SERVERCONFIG_HPP
# include <vector>
# include <iostream>
# include "Location.hpp"


class ServerConfig
{
	public:
		std::string server_name;
		std::vector< std::pair<int, std::string> > error_pages;
		int client_body_size;
		std::vector<Location> location;
		ServerConfig();
		ServerConfig( const ServerConfig & serverConf );
		ServerConfig & operator=( const ServerConfig & serverConf );
		~ServerConfig();
};

// we could make error_pages a pair of vector that has the status and path

#endif
