#include "webserv.hpp"

Multiplex::Multiplex()
{

}

Multiplex::Multiplex( const Multiplex & multiplex )
{
	*this = multiplex;
}

Multiplex & Multiplex::operator=( const Multiplex & multiplex )
{
	if ( this != &multiplex )
	{
		std::cout << "copy assinment multiplex";
	}
	return ( *this );
}

Multiplex::~Multiplex()
{

}

void Multiplex::execute( const Server & server )
{
	setup( server.serverInfo );
	multiplexing( server );
}

void Multiplex::setup( const std::vector< ServerInfo > & serverInfo )
{
	size_t i = 0;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&tmp_readfds);
	FD_ZERO(&tmp_writefds);
	while ( i < serverInfo.size() )
	{
		FD_SET( serverInfo[i].id, &tmp_readfds );
		i++;
	}
	nfds = serverInfo[ serverInfo.size() - 1 ].id;
}

bool Multiplex::is_socket( const std::vector< ServerInfo > & serverInfo, int socket )
{
	for ( size_t i = 0; i < serverInfo.size(); i++ )
	{
		if ( socket == serverInfo[i].id )
		{
			return ( true );
		}
	}
	return ( false );
}

void Multiplex::multiplexing( const Server & server )
{
	(void)server;
	while (1)
	{
		readfds = tmp_readfds;
		writefds = tmp_writefds;
		if ( select( nfds + 1, &readfds, &writefds, NULL, NULL ) == -1 )
		{
			perror( "select error:" );
		}
		for ( int i = 0; i <= nfds; i++ )
		{
			if ( FD_ISSET( i, &readfds ) )
			{
				if ( is_socket( server.serverInfo, i ) )
				{
					std::cout << "accept\n";
					newfd = accept( i, NULL, NULL );
					if ( newfd == -1 )
					{
						perror( "accept error: " );
						break ;
					}
					else
					{
						//std::cout << "socket: " << i << std::endl;
						//std::cout << "newfd: " << newfd << std::endl;
						FD_SET( newfd, &tmp_readfds );
						nfds = std::max(newfd, nfds);
						//std::cout << "add: " << newfd << std::endl;
						client.addClient( ClientInfo( newfd, server.getServer( i ) ) );
					}
				}
				else
				{
					memset( recv_buffer, '\0', sizeof( recv_buffer ) );
					rec = recv( i,  recv_buffer, sizeof( recv_buffer ), 0 );
					if ( rec <= 0 )
					{
						// handle error
						if ( rec == 0 )
						{
							std::cout << "connection closed\n";
						}
						else
							perror( "receive error: " );
						client.removeClient( i );
						close( i );
						FD_CLR( i, &tmp_readfds );
						FD_CLR( i, &tmp_writefds );
						break ;
					}
					else
					{
						recv_buffer[rec] = '\0';
						try {
							client.setup( recv_buffer, i );
						}
						catch ( int e )
						{
							FD_SET( i, &tmp_writefds );
						}
					}
				}
			}
			else if ( FD_ISSET( i, &writefds ) )
			{
				send_buffer = client.result( i );
				/*send_buffer = client.get_response( i );*/
				sen = send( i, send_buffer.c_str(), send_buffer.length(), 0 );
				if ( sen == -1 )
				{
					perror( "send error: " );
				}
				// maybe remove the id or the node because you finished from it.
				// you can do the same in reading
				//std::cout << "remove: " << i << std::endl;
				client.removeClient( i );
				FD_CLR( i, &tmp_writefds );
				FD_CLR( i, &tmp_readfds );
				close(i);
			}
		}
	}
}
// Notes:
// set the socket on the writefds only when you finished receiving request
