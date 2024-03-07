#include "webserv.hpp"

Request::Request()
{
	statusCode = 0;
}

Request::Request( const Request & request )
{
	*this = request;
}

Request & Request::operator=( const Request & request )
{
	if ( this != &request )
	{
		//std::cout << "copy assignment request\n";
		this->statusCode = request.statusCode;
		this->method = request.method;
		this->path = request.path;
		this->protocol = request.protocol;
		this->header = request.header;
		this->body = request.body;
	}
	return ( *this );
}

Request::~Request()
{

}

// maybe add a host variable because it's gonna make it easy for me in response
void Request::setup( const std::string & str )
{
	(void)str;
	std::cout << str;
	this->method = "POST";
	this->path = "/cgi-bin/";
	this->protocol = "HTTP/1.1";
	this->header.push_back( std::make_pair( "Host", "localhost:800" ) );
	this->body = "name=oussama";
	/*handleMethod();
	handlePath();
	handleProtocol();
	handleHeaders();
	handleBody();*/
	if ( str.find("\r\n\r\n") != std::string::npos )
		throw (200);
}

/*void Request::print()
{
	std::cout << "method: " << method << std::endl;
	std::cout << "path: " << path << std::endl;
	std::cout << "protocol: " << protocol << std::endl;
	for ( size_t i = 0; i < header.size(); i++ )	
	{
		std::cout << header[i].first << " | " << header[i].second;
	}
}

void Request::fill_method_path_protocol( std::stringstream & ss )
{
	std::string tmp;

	if ( ss >> tmp )
		method = tmp;
	if ( ss >> tmp )
		path = tmp;
	if ( ss >> tmp )
		protocol = tmp;
	if ( ss >> tmp )
	{
		throw ( 400 );
	}
}

bool Request::isUpperCase( const std::string & str )
{
	for ( size_t i = 0; i < str.length(); i++ )
	{
		if ( !std::isupper( str.at( i ) ) )
			return ( false );
	}
	return ( true );
}

void Request::check_method_path_protocol()
{
	if ( !method.length() || !path.length() || !protocol.length() )
	{
		throw ( 400 );
	}
	else if ( !isUpperCase( method ) )	
	{
		throw ( 400 );
	}
	else if ( path.at( 0 ) != '/')
	{
		throw ( 400 );
	}
	else if ( protocol != "HTTP/1.1" )
	{
		throw ( 400 );
	}
}

void Request::parse_method_path_protocol( const std::string & str )
{
	std::stringstream ss( str );

	fill_method_path_protocol( ss );
	check_method_path_protocol();
	headerParseIndex++;
}

std::string Request::get_key( const std::string & str )
{
	std::string key;
	std::size_t pos;

	pos = str.find( ":" );
	if ( pos != std::string::npos )
	{
		key = str.substr(0, pos);
	}
	else
	{
		key = str;
	}
	return ( key );
}
std::string Request::get_value( const std::string & str )
{
	std::string value;
	std::size_t pos;

	pos = str.find( ":" );
	if ( pos != std::string::npos )
		value = str.substr(pos + 1);
	return ( value );
}*/

/*bool Request::check_key( const std::string & str )
{

}

bool Request::check_value( const std::string & str )
{

}*/

/*void Request::parse_header( const std::string & str )
{
	std::string key;
	std::string value;

	key = get_key( str );
	value = get_value( str );
	header.push_back( std::make_pair( key, value ) );
	if ( check_key( key ) )
	{

	}
	if ( check_value( value ) )
	{

	}
}

void Request::parse_request_header( const std::string & str )
{
	if ( str == "\r\n" )
		return ;
	if ( headerParseIndex == 0 )
		parse_method_path_protocol( str );
	else
		parse_header( str );
}

void Request::parse_request_body( const std::string & str )
{
	(void)str;
	if ( isChunked )
	{
		//parse_chunked_request();
	}
	else
	{
		//parse_length_request();
	}
}
// content length and the chunked

void Request::parse_request( const std::string & str )
{
	if ( !inBody )
	{
		//std::cout << "in here\n";
		parse_request_header( str );
	}
	else
	{
		parse_request_body( str );
	}
}

void Request::clear()
{
	status = 0;
	method.clear();	
	path.clear();
	protocol.clear();
	header.clear();
	body.clear();
	contentLength = 0;
	chunkLength = 0;
	isChunked = false;
	inBody = false;
	headerParseIndex = 0;
}

bool Request::key_exist( const std::string & key )
{
	for ( size_t i = 0; i < header.size(); i++ )
	{
		if ( key == header[i].first )
			return ( true );
	}
	return ( false );
}

void Request::check_header_info()
{
	if ( !key_exist( "Host" ) )
	{
		throw ( 400 );
	}
	else if ( method == "POST" )
	{
		if ( key_exist( "Content-Length" ) && key_exist( "Transfer-Encoding" ))
		{
			std::cout << "t\n";
			throw ( 400 );
		}
		else if ( !key_exist( "Content-Length" ) && !key_exist( "Transfer-Encoding" ) )
			throw ( 400 );
	}
}*/

// parse the request
// i need to start thinking about the response and how to handle it
// maybe do an enum for the error code that will go with a status code
// you should check the special characters allowed in http request
// the key headers can be anything , it dones't need : or value or something
// key always attached to : , the value doesn't matter
// key is necessary to have in case of ":value"
// key can only accept printable characters no space no tab no weird stuff
// value accept everything
