#include "webserv.hpp"

Request::Request()
{
	statusCode = 0;
	std::cout << "creation\n";
	index = 0;
	inBody = false;
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
		this->lines = request.lines;
		this->index = request.index;
		this->rem = request.rem;
		this->inBody = request.inBody;
		this->contentLength = request.contentLength;
	}
	return ( *this );
}

Request::~Request()
{

}

// i need to check if it reached the end
// when the header is finished i need to parse and see if the necessary is there
bool Request::isUpperCaseLetters( const std::string & str )
{
	for ( size_t i = 0; i < str.length(); i++ )
	{
		if ( !std::isupper( str.at( i ) ) || !std::isalpha( str.at( i ) ) )
			return ( false );
	}
	return ( true );
}

std::vector<std::string> split( const std::string str )
{
	std::stringstream ss;
	std::string tmp;
	std::vector<std::string> result;

	ss << str;
	while ( ss >> tmp )
	{
		result.push_back( tmp );
	}
	return ( result );
}

void Request::handleMethod( const std::string & str )
{
	if ( !isUpperCaseLetters( str ) )
	{
		std::cerr << "not upper and letter\n";
		throw ( 400 );
	}
	method = str;
}

void Request::handlePath( const std::string & str )
{
	if ( str.at( 0 ) != '/' )
	{
		std::cerr << "path not starting with /\n";
		throw ( 400 );
	}
	path = str;
}

void Request::handleProtocol( const std::string & str )
{
	if ( str != "HTTP/1.1" )
		throw ( 505 );
	protocol = str;
}

// to be improved
void Request::handleRequestLine()
{
	std::vector<std::string> result;

	if ( index != 0 )
		return ;
	result = split( lines[ index ] );
	if ( result.size() < 3 )
	{
		std::cerr << "less than requestline\n";
		throw ( 400 );
	}
	handleMethod( result[ 0 ] );
	handlePath( result [ 1 ] );
	handleProtocol( result[ 2 ] );
	if ( result.size() != 3 )
	{
		std::cerr << "more than requestline\n";
		throw ( 400 );
	}
}
std::string Request::getKey( const std::string & str )
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
std::string Request::getValue( const std::string & str )
{
	std::string value;
	std::size_t pos;

	pos = str.find( ":" );
	if ( pos != std::string::npos )
		value = str.substr(pos + 1);
	return ( value );
}

void Request::handleHeaders()
{
	if ( index == 0 )
		return ;
	header.push_back( std::make_pair( getKey( lines[ index ] ), getValue( lines[ index ] ) ) );
}

int Request::getContentLength()
{
	for ( size_t i = 0; i < header.size(); i++ )
	{
		if ( header[ i ].first == "Content-Length" )
		{
			return ( std::atoi( header[ i ].second.c_str() ) );
		}
	}
	return ( -1 );
}

void Request::isEnd()
{
	if ( lines[ index ] == "\r\n" )
	{
		if ( index != 0 && lines[ index - 1] != "\r\n" )
		{
			contentLength = getContentLength();
			// check content-length or transfer-encoding
			// check host and all necessary stuff for header
			if ( method != "POST" )	
				throw ( 200 );
			inBody = true;
		}
	}
}

/*void Request::handleBody( const std::string & str )
{
	if ( !inBody )
		return ;
	body += str
}*/

void Request::parse()
{
	handleRequestLine();
	handleHeaders();
	isEnd();
}

// i need to store all the headers in lower_case
// maybe add a host variable because it's gonna make it easy for me in response
// there is a problem with str
void Request::headerFill( const std::string & str )
{
	std::string line;
	std::size_t pos;

	if ( inBody )
		return ;
	if ( !rem.empty() )
	{
		line = rem;
		rem.clear();
	}
	for ( size_t i = 0; i < str.length(); i++ )
	{
		line += str.at( i );
		pos = line.find( "\r\n" );
		if ( pos != std::string::npos )
		{
			lines.push_back( line );
			parse();
			index++;
			line.clear();
		}
	}
	str.clear();
	if ( !line.empty() )
	{
		rem = line;
	}
}

void Request::bodyFill( const std::string & str )
{
	if ( !inBody )
		return ;
	body += str;
	//handleContentLength();
	//handleChunked();
	if ( body.length() > contentLength )
	{
		std::cout << "|" << body << "|";
		std::cout << contentLength << std::endl;
		std::cout << body.size() << std::endl;
		std::cerr << "body content comparaison\n";
		throw ( 400 );
	}
	else if ( body.length() == contentLength )
		throw ( 200 );
}


void Request::setup( const std::string & str )
{
	try {
		headerFill( str );	
		bodyFill( str );
	}
	catch ( int & e )
	{
		print();
		statusCode = e;
		std::cout << e << std::endl;
		throw( e );
	};
}

void Request::print()
{
	std::cout << "method: " << method << std::endl;
	std::cout << "path: " << path << std::endl;
	std::cout << "protocol: " << protocol << std::endl;
	for ( size_t i = 0; i < header.size(); i++ )	
	{
		std::cout << "header: " << header[i].first << ":" << header[i].second;
	}
	std::cout << body << std::endl;
}

void Request::clear()
{
	std::cout << "clear\n";
	statusCode = 0;
	method.clear();	
	path.clear();
	protocol.clear();
	header.clear();
	body.clear();
	lines.clear();
	index = 0;
}

/*void Request::fill_method_path_protocol( std::stringstream & ss )
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
