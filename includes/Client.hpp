#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "ClientInfo.hpp"
# include <vector>

class Client
{
	public:
		std::vector<ClientInfo> clientInfo;
		Client();
		Client(const Client & client);
		Client & operator=(const Client & client);
		~Client();
		int getClientInfoIndex( int id );
		void addClient( ClientInfo newClientInfo );
		void removeClient( int id );
		void setup( const std::string & str, int id );
		//std::string get_response( int id );
};

#endif
