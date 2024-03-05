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
void ClientInfo::setup( const std::string & str )
{
	try {
		//std::cout << str;
		request.setup( str );
	}
	catch ( int e )
	{
		std::cout << "catch: " << e << std::endl;
		request.statusCode = e;
		response.setup( request, serverInfo );
		throw ( 1 );
	}
}

// add headers
// get mimetypes
// get the status based on the code
// don't touch anything that is related with request because it's already cleared
// BE CAREFUL : std::to_string C++11
/*std::string ClientInfo::get_response()
{
	std::string body = std::string( response.body.begin(), response.body.end() ); 
	std::string send_buffer = "HTTP/1.1 " + std::to_string( response.code ) + " OK\r\n";
	send_buffer += "Content-Length: " + std::to_string( body.length() ) + "\r\n";
	for ( size_t i = 0; i < response.header.size(); i++ )
	{
		send_buffer += response.header[i].first + ": " + response.header[i].second + "\r\n";
	}
	send_buffer += "\r\n";
	send_buffer += body;
	std::cout << send_buffer;
	response.clear();
	return ( send_buffer );
}*/
// need to clear everything
// i should think about a way to handle POST
// when i respond i should reset the lines and the index
