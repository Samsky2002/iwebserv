#include "webserv.hpp"

Response::Response()
{

}
Response::Response( const Response & response)
{
	*this = response;
}

Response & Response::operator=( const Response & response)
{
	if ( this != &response )
	{
		//std::cout << "copy assignment response\n";
		this->protocol = response.protocol;
		this->statusCode = response.statusCode;
		this->status = response.status;
		this->header = response.header;
		this->body = response.body;
		this->finalBody = response.finalBody;
		this->serverConfig = response.serverConfig;
		this->location = response.location;
		this->resource = response.resource;
		this->resourceType = response.resourceType;
	}
	return ( *this );
}

Response::~Response()
{

}

void Response::resourceHandle()
{
	struct stat path_stat;
	int result;

	if ( stat( resource.c_str(), &path_stat ) != 0)
	{
		std::cout << resource << std::endl;
		perror( "stat: " );
		throw ( 404 );
	}
	result = S_ISDIR( path_stat.st_mode );
    if (result == 1) {
		resourceType = Directory;
    } else if (result == 0) {
		resourceType = File;
    }
}

// to check this, understand it better
void Response::fillBody()
{
	std::stringstream ss;
	std::ifstream inputFile( resource, std::ios::binary );
	std::string line;
	//std::string test;

	if ( inputFile.is_open( ) )
	{
		ss << inputFile.rdbuf();
		line = ss.str();
		body = std::vector<int>(line.begin(), line.end());
		inputFile.close();
		//test = std::string ( body.begin(), body.end() );
		//std::cout << test << std::endl;
	}
	else
	{
		std::cerr << "couldn't open file\n";
		throw ( 404 );
	}
}

bool Response::is_url_found_in_path( const std::string & requestPath, const std::string & locationPath )
{
	std::size_t found;

	if ( locationPath.length() > requestPath.length() )
	{
		return ( false );
	}
	found = locationPath.find( requestPath.substr( 0, locationPath.length() ) );
	if ( found != std::string::npos )
	{
		return ( true );
	}
	return ( false );
}

void Response::urlHandle( std::string requestPath)
{
	bool found;

	for ( size_t i = 0; i < serverConfig.location.size(); i++ )
	{
		found = is_url_found_in_path( requestPath, serverConfig.location[i].path );
		if ( found )
		{
			if ( !location.path.empty() )
			{
				if ( location.path.length() < serverConfig.location[i].path.length() )
					location = serverConfig.location[i];
			}
			else
				location = serverConfig.location[i];
		}
	}
	if ( location.path.empty() )
		throw ( 404 );
	resource = requestPath.replace( 0, location.path.length(), location.root );
}

void Response::redirectionHandle()
{
	if ( location.redirection.empty() )
		return ;
	header.push_back( std::make_pair( "Location", location.redirection ) );
	throw ( 301 );
}

void Response::methodHandle( const Request & request )
{
	if ( request.method != "GET" && request.method != "POST" && request.method != "DELETE" )
		throw ( 405 );
	if ( location.methods.empty() )
		return ;
	for ( size_t i = 0; i < location.methods.size(); i++ )
	{
		if ( request.method == location.methods[i] )
			return ;
	}
	throw ( 405 );
}

void Response::launchCgi()
{
	int fd[ 2 ];
	int pid;
	char *str = strdup(  );
	char *arr[] = {str, NULL};

	if ( pipe( fd ) )
		std::cout << "pipe error\n";
	pid = fork();
	if ( pid == 0 )
	{
		if ( dup2(fd[ 1 ], 1) )
			perror( "dup2" );
		close( fd );
		if ( execve( resource.c_str(), arr, NULL ) == -1 )
		{
			perror( "execve" );
			exit(1);
		}
	}
	wait( NULL );
	close( fd );
}

void Response::setServerConfig( const Request & request, const ServerInfo & serverInfo )
{
	std::string host;

	(void)serverInfo;
	// this is just a temp solution
	for ( size_t i = 0; i < request.header.size(); i++ )
	{
		if ( request.header[i].first == "Host" )
			host = request.header[i].second;
	}
	host = host.substr( 0, host.find(":") );
	if ( serverInfo.host == host )
	{
		serverConfig = serverInfo.serverConfig[0];	
		return ;
	}
	for ( size_t i = 0; i < serverInfo.serverConfig.size(); i++ )
	{
		if ( serverInfo.serverConfig[ i ].server_name == host )
		{
			serverConfig = serverInfo.serverConfig[ i ];
			return ;
		}
	}
	serverConfig = serverInfo.serverConfig[ 0 ];
}

bool Response::hasIndexHtml()
{
	std::string tmp;

	tmp = resource + "index.html";
	if ( !access( tmp.c_str(), F_OK ) )
	{
		resource = tmp;
		return ( true );
	}
	return ( false );
}

bool Response::hasIndex()
{
	std::string tmp;

	if ( location.index.empty() )
	{
		if ( hasIndexHtml() )
			return ( true );
		return ( false );
	}
	for ( size_t i = 0; i < location.index.size(); i++ )
	{
		tmp = resource;
		tmp += location.index[i];
		if ( !access( tmp.c_str(), F_OK ) )
		{
			//std::cout << "index: " << location.index[i] << std::endl;
			resource = tmp;
			return ( true );
		}
	}
	return ( false );
}

bool Response::hasTrailingSlach()
{
	if ( resource.at( resource.length() - 1 ) == '/' )
		return ( true );
	return ( false );
}

void Response::directoryHandle( const Request & request )
{
	if ( resourceType != Directory )
		return ;
	if ( !hasTrailingSlach() )
	{
		header.push_back( std::make_pair( "Location", request.path + "/" ) );
		throw ( 301 );
	}
	if ( hasIndex() )
		return ;
	// autoindex
	throw ( 403 );
}

bool Response::isCgi()
{
	if ( location.path == "/cgi-bin" || location.path == "/cgi-bin/" )
		return ( true );
	return ( false );
}

void Response::cgiHandle()
{
	if ( !isCgi() )
		return ;
	throw( 200 );
	// handle cgi
}

void Response::fileHandle( const Request & request )
{
	if ( request.method == "POST" || request.method == "DELETE")
		throw ( 405 );
	fillBody();
	//std::cout << "resource: " << resource << std::endl;
}

void Response::setStatus()
{
	if ( statusCode == 200 )
		status = "OK";
	else if ( statusCode == 404 )
		status = "Not Found";
	else if ( statusCode == 301 )
		status = "Moved Permanently";
	else if ( statusCode == 405 )
		status = "Not Allowed";
	else if ( statusCode == 403 )
		status = "Forbidden";
}

std::string Response::getExtension()
{
	size_t pos;
	std::string extension;

	pos = resource.find_last_of( '.' );
	if ( pos != std::string::npos )
	{
		extension = resource.substr( pos + 1 );
	}
	return ( extension );
}

std::string Response::getContentType()
{
	std::string extension = getExtension();

	// this might be wrong i need to think about it
	if ( resource.empty() )
		return ( "text/html" );
	if ( extension == "html" )
		return ( "text/html" );
	return ( "text/plain" );
}

void Response::setHeaders()
{
	header.push_back( std::make_pair( "Content-Type", getContentType() ) );
	header.push_back( std::make_pair( "Content-Length", std::to_string( finalBody.length() ) ) );
}

/*void Response::setBody()
{
	finalBody = std::string( body.begin(), body.end() );
}*/

void Response::fillDefaultErrorPage()
{
	finalBody = "<html><head><title>" + std::to_string( statusCode ) + " " + status + "</title></head>";
	finalBody += "<body><center><h1>" + std::to_string( statusCode ) + " " + status + "</h1></center>";
	finalBody += "<hr><center>webserv/1.1</center></body></html>";
}

void Response::errorResponse()
{
	bool found;

	found = false;
	if ( statusCode == 200 || statusCode == 301) // succes code not just 200 - maybe make a success function
		return ;
	for ( size_t i = 0; i < serverConfig.error_pages.size(); i++ )
	{
		if ( serverConfig.error_pages[i].first == statusCode )
		{
			resource = serverConfig.error_pages[i].second;
			found = true;
		}
	}
	if ( found )
	{
		fillBody();
		finalBody = std::string( body.begin(), body.end() );
	}
	else
	{
		resource.clear(); // temporary solution to know that i have default error page maybe change it
		fillDefaultErrorPage();
	}
}

/*void Response::setBody()
{
	if (  )
	fillDefaultErrorPage();
	finalBody = std::string( body.begin(), body.end() );
}*/

void Response::response()
{
	protocol = "HTTP/1.1";
	setStatus();
	finalBody = std::string( body.begin(), body.end() );
	errorResponse();
	//setBody();
	setHeaders();
}

std::string Response::result()
{
	std::string result;

	result = protocol + " " + std::to_string( statusCode ) + " " + status + "\r\n";
	for ( size_t i = 0; i < header.size(); i++ )
	{
		result += header[i].first + ": " + header[i].second + "\r\n";
	}
	result += "\r\n";
	result += finalBody;
	std::cout << result;
	return ( result );
}

void Response::setup( const Request & request, const ServerInfo & serverInfo )
{
	(void)serverInfo;
	// i need to get the proper config
	statusCode = request.statusCode;
	setServerConfig( request, serverInfo );
	try {
		if ( statusCode != 200 )
			throw ( statusCode );
		urlHandle( request.path );
		redirectionHandle();
		methodHandle( request );
		resourceHandle();
		directoryHandle( request );
		cgiHandle();
		fileHandle( request );
		//check resource and check cgi
		//launch( request, serverConfig );
		throw (200);
	}
	catch ( int i )
	{
		statusCode = i;
		std::cout << "statusCode: " << statusCode << std::endl;
		response();
	}
}
// if bad response
// is there an error file that goes with that response
// read that file
// fill the body
// get the extension and stuff

/*void Response::clear()
{
	protocol.clear();
	code = 0;
	status.clear();
	header.clear();
	body.clear();
	resource.clear();
	locationIndex = 0;
	resourceType = 0;
}*/
// check location url
// check redirection
// check protocol
// check method and start going to what method you have
// check resource type and if it exist
