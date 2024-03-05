#include "webserv.hpp"

Client::Client()
{

}

Client::Client(const Client & client)
{
	*this = client;
}

Client & Client::operator=(const Client & client)
{
	if ( this != &client )
	{
		//std::cout << "copy assignment client\n";
		this->clientInfo = client.clientInfo;
	}
	return ( *this );
}

Client::~Client()
{

}

void Client::setup( const std::string & str, int id )
{
	for ( size_t i = 0; i < clientInfo.size(); i++ )
	{
		if ( clientInfo[i].id == id )
		{
			clientInfo[i].setup( str );
		}
	}
}

/*std::string Client::get_response( int id )
{
	std::string tmp;

	for ( size_t i = 0; i < clientInfo.size(); i++ )
	{
		if ( clientInfo[i].id == id )
		{
			return ( clientInfo[i].get_response() );
		}
	}
	std::cerr << "get_response problem\n";
	return (tmp);
}*/

void Client::removeClient( int id )
{
	for ( size_t i = 0; i < clientInfo.size(); i++ )
	{
		if ( id == clientInfo[ i ].id )
		{
			clientInfo.erase( clientInfo.begin() + i );
			return ;
		}
	}
}

void Client::addClient( ClientInfo newClientInfo )
{
	clientInfo.push_back( newClientInfo );
}

int Client::getClientInfoIndex( int id )
{
	size_t i = 0;

	while ( i < clientInfo.size() )
	{
		if ( clientInfo[ i ].id == id )
			return ( i );
		i++;
	}
	return ( i );
}
