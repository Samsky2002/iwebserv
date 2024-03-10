#include "webserv.hpp"

ClientInfo::ClientInfo()
{
}

ClientInfo::ClientInfo( int id, const ServerInfo & serverInfo )
{
	this->id = id;
	this->serverInfo = serverInfo;
}

ClientInfo::ClientInfo(const ClientInfo & clientInfo)
{
	*this = clientInfo;
}

ClientInfo & ClientInfo::operator=(const ClientInfo & clientInfo)
{
	if ( this != &clientInfo )
	{
		//std::cout << "copy assignment clientInfo\n";
		this->id = clientInfo.id;
		this->request = clientInfo.request;
		this->response = clientInfo.response;
		this->serverInfo = clientInfo.serverInfo;
	}
	return ( *this );
}

ClientInfo::~ClientInfo()
{

}

/*bool ClientInfo::reach_end()
{
	if ( linesIdx != 0 && linesIdx < lines.size() && lines[ linesIdx ] == "\r\n" && lines[ linesIdx - 1 ] != "\r\n")
	{
		request.inBody = true;
		request.check_header_info();
		// i need to check if the header has all necessary info
		if ( request.method != "POST" )
			throw ( 200 );
	}
	if ( is_post_method() && request.reachEnd )
	{
		throw ( 1 );
	}
	return ( false );
}
// maybe do a varialbe that tells you that you finished from the header and now you are in the body
// when it's finished it checks if every variable is there

void ClientInfo::request_buffer_prep()
{
	std::string line;
	std::size_t pos;

	if ( !rem.empty() )
	{
		line = rem;
		rem.clear();
	}
	for ( size_t i = 0; i < requestBuffer.length(); i++ )
	{
		line += requestBuffer.at( i );
		pos = line.find( "\r\n" );
		if ( pos != std::string::npos )
		{
			std::cout << lines[i];
			lines.push_back( line );
			request.parse_request( lines[ linesIdx ] );
			//reach_end();
			linesIdx++;
			line.clear();
		}
	}
	if ( !line.empty() )
	{
		rem = line;
	}
}*/

// this isn't the best approach
void ClientInfo::setup( std::vector<char> & buffer )
{
	try {
		request.setup( buffer );
	}
	catch ( int e )
	{
		response.setup( request, serverInfo );
		buffer.clear();
		request.clear();
		throw ( 1 );
	}
}

// BE CAREFUL : std::to_string C++11
std::string ClientInfo::result()
{
	return ( response.result() );
}
// need to clear everything
