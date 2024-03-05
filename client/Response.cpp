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
		this->serverConfig = response.serverConfig;
	}
	return ( *this );
}

Response::~Response()
{

}

/*void Response::resourceHandle(const char * path)
{
	struct stat path_stat;
	int result;

	if ( stat( path, &path_stat ) != 0)
	{
		perror( "stat: " );
		std::cout << path;
		throw ( 404 );
	}
	result = S_ISDIR( path_stat.st_mode );
    if (result == 1) {
		resourceType = 1;
    } else if (result == 0) {
		resourceType = 0;
    }
}*/

/*bool Response::index_exist()
{
	DIR *directory = opendir( resource.c_str() );

	if ( !directory )
	{
		perror( "opendir: " );
		throw ( 404 );
	}
	struct dirent *entry;
	while ( (entry = readdir( directory )) != NULL)
	{
		if ( strcmp( entry->d_name, "index.html" ) == 0 )
			return ( true );
	}
	return ( false );
}

void Response::fill_body()
{
	std::string file = resource;
	std::stringstream ss;
	std::ifstream inputFile( file, std::ios::binary );
	std::string line;

	if ( inputFile.is_open( ) )
	{
		ss << inputFile.rdbuf();
		line = ss.str();
		body = std::vector<int>(line.begin(), line.end());
	}
	else
	{
		std::cerr << "couldn't open file\n";
		throw ( 404 );
	}
	inputFile.close();
}*/

/*void Response::get_resource()
{
	if ( resourceType )
	{
		// directory
		if ( index_exist() )
		{
			fill_body();
		}
	}
	else
	{
		//file
	}
}*/

bool Response::is_url_found_in_path( const std::string & requestPath, const std::string & locationPath  )
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

/*std::pair<std::string, int> Response::getFinalLocationPath( const std::vector<std::pair<std::string, int> > & locationFound )
{
	int maxLen;
	int len;
	int idx;

	maxLen = locationFound[0].first.length();
	len = 0;
	idx = 0;
	for ( size_t i = 0; i < locationFound.size(); i++ )
	{
		len = locationFound[i].first.length();
		if ( len > maxLen )
		{
			maxLen = len;
			idx = i;
		}
	}
	return ( locationFound[idx] );
}*/

void Response::urlHandle( std::string requestPath)
{
	bool found;
	//std::vector<std::pair<std::string, int> > locationFound;
	//std::pair<std::string, int> finalLocationPath;

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
			//locationFound.push_back( std::make_pair( serverConfig.location[i].path, i ) );
		}
	}
	if ( location.path.empty() )
		throw ( 404 );
	//finalLocationPath = getFinalLocationPath( locationFound );
	//locationIndex = finalLocationPath.second;
	std::cout << requestPath << std::endl;
	resource = requestPath.replace( 0, location.path.length(), location.root );
	std::cout << resource << std::endl;
}

void Response::redirectionHandle()
{
	if ( location.redirection.empty() )
		return ;
	header.push_back( std::make_pair( "Location", serverConfig.location[ locationIndex ].redirection ) );
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

/*void Response::handle_file()
{
	fill_body();
	// check cgi
}

bool Response::has_trailing_slach()
{
	if ( resource.at( resource.length() - 1 ) == '/' )
		return ( true );
	return ( false );
}

void Response::handle_directory( const Request & request, const ServerConfig & serverConfig )
{
	if ( !has_trailing_slach() )
	{
		header.push_back( std::make_pair( "Location", request.path + "/" ) );
		throw ( 301 );
	}
	else if ( !serverConfig.location[ locationIndex ].index.empty() )
	{
		//get_index();
	}
	else if ( index_exist() )
	{
		resource += "index.html";
	}
	else if ( serverConfig.location[ locationIndex ].autoindex )
	{
		// handle autoindex
		std::cout << "there is an auto index\n";
	}
	else
		throw ( 403 );
}

void Response::handle_get( const Request & request, const ServerConfig & serverConfig )
{
	if ( resourceType == 1 )
	{
		std::cout << "directory\n";
		handle_directory( request, serverConfig );
		// file
	}
	if ( cgi )
	{
		handle_cgi();
		// cgi
	}
	else
	{
		handle_file();
	}
}

void Response::handle_post(  )
{
	if ( !cgi )
	{
		// not allowed
		throw ( 405 );
	}
	if ( resourceType == 0)
	{
		std::cout << "file\n";
	}
	else
	{
		std::cout << "directory\n";
	}
}

bool Response::is_cgi()
{
	// check if it's php script
	if ( resource.find( "/cgi-bin/" ) != std::string::npos )
	{
		return ( true );
	}
	return ( false );
}

void Response::handle_cgi()
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

void Response::launch( const Request & request, const ServerConfig & serverConfig )
{
	get_resource_type( resource.c_str() );
	if ( is_cgi() )
		cgi = true;
	if ( request.method == "GET" )
	{
		handle_get( request, serverConfig );
	}
	else if ( request.method == "POST" )
	{
		handle_post();
	}
	else if ( request.method == "DELETE" )
		handle_delete();
}*/

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
	std::cout << host << std::endl;
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
		//resourceHandle();
		//check resource and check cgi
		//launch( request, serverConfig );
		throw (200);
	}
	catch ( int i )
	{
		std::cout << statusCode << std::endl;
		//response();
	}
}

void Response::response()
{
	protocol = "HTTP/1.1";
	//fill_status();
	//fill_header();
}

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
