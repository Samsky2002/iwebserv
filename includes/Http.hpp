#ifndef HTTP_HPP
# define HTTP_HPP
# include <vector>
# include "Server.hpp"
# include "Multiplex.hpp"
//# include "Client.hpp"

class Http
{
	public:
		Server server;
		Multiplex multiplex;
		//Client client;
		std::string config_file;
		Http();
		Http( const Http & http );
		Http & operator=( const Http & http );
		~Http();
		void execute(const std::string & file);
};

#endif
