#include "webserv.hpp"
// he will need the body as an input file
// script path
// check extension of the script
// environment

void Cgi::setupEnv()
{
	env = ( char ** )malloc( 2 * sizeof( char * ) );
	env[0] = strdup("REQUEST_METHOD=GET");
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

void Cgi::setupArgv()
{
	path = strdup( "/usr/bin/python" );
	argv = ( char ** )malloc( 3 * sizeof( char * ) );
	argv[0] = strdup("python");
	argv[1] = strdup( response.resource.c_str() );
	argv[2] = NULL;
}

void Cgi::setup( const Response & response )
{
	(void)response;
	setupEnv();
	setupArgs();
}

// maybe put the input file in a pipe
void Cgi::launch()
{
	if ( pipe( fd ) == -1 )
		perror( "pipe" );
	pid = fork();
	if ( pid == 0 )
	{
		std::cout << path << std::endl;
		close( fd[ 0 ] );
		if ( dup2(fd[ 1 ], 1) == -1 )
			perror( "dup2" );
		close( fd[ 1 ] );
		if ( execve( path, argv, NULL ) == -1 )
		{
			perror( "execve" );
			exit(1);
		}
	}
	close( fd[ 1 ] );
	if ( wait( NULL ) == -1 )
		perror("wait: ");
	while ( ( bytesRead = read( fd[ 0 ], rbuff, sizeof( rbuff ) - 1 ) ) > 0 )
	{
		rbuff[ bytesRead ] = '\0';
		body += rbuff;
	}
	close( fd[ 0 ] );
	std::cout << body << std::endl;
}
