#include "webserv.hpp"
// he will need the body as an input file
// script path
// check extension of the script
// environment

void Cgi::setupEnv()
{
	env = ( char ** )malloc( 2 * sizeof( char * ) );
	env[0] = strdup("REQUEST_METHOD=POST");
	//env[1] = strdup("CONTENT_LENGTH=9");
	env[1] = NULL; 
}
// query_string
// request_method
// content_type
// content_length
// script_name
// path_info
// upload_path
// 

void Cgi::setupArgv( const Response & response )
{
	path = strdup( "/usr/bin/python" );
	argv = ( char ** )malloc( 3 * sizeof( char * ) );
	argv[0] = strdup("python");
	argv[1] = strdup( response.resource.c_str() );
	argv[2] = NULL;
}

void Cgi::setupInputFile( const Request & request )
{
	int fd_tmp[2];

	if ( pipe( fd_tmp ) == -1 )
	{
		perror("fd_tmp");
		return ;
	}
	char *tmp = new char[ request.body.length() + 1 ];
	std::copy( request.body.begin(), request.body.end(), tmp );
	tmp[ request.body.length() ] = '\0';
	int bytes = write( fd_tmp[ 1 ], tmp, request.body.length() );
	if ( bytes == -1 )
		perror("write");
	else
		std::cout << "Nice\n";
	close ( fd_tmp[ 1 ] );
	input_fd = fd_tmp[ 0 ];
	delete[] tmp;
}

void Cgi::setup( const Request & request, const Response & response )
{
	( void ) request;
	setupEnv();
	setupArgv( response );
	setupInputFile( request );
}

// maybe put the input file in a pipe
void Cgi::launch()
{
	if ( pipe( fd ) == -1 )
		perror( "pipe" );
	pid = fork();
	if ( pid == 0 )
	{
		close( fd[ 0 ] );
		if ( dup2( input_fd, 0 ) == -1 )
			perror( "dup2" );
		if ( dup2( fd[ 1 ], 1 ) == -1 )
			perror( "dup2" );
		close( fd[ 1 ] );
		close( input_fd );
		if ( execve( path, argv, env ) == -1 )
		{
			perror( "execve" );
			exit(1);
		}
	}
	close( fd[ 1 ] );
	close( input_fd );
	if ( wait( NULL ) == -1 )
		perror("wait: ");
	while ( ( bytesRead = read( fd[ 0 ], rbuff, sizeof( rbuff ) - 1 ) ) > 0 )
	{
		rbuff[ bytesRead ] = '\0';
		body += rbuff;
	}
	close( fd[ 0 ] );
	//std::cout << body << std::endl;
}
