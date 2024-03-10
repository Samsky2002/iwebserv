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
	std::string str;

    FILE* tempFile = tmpfile();

    if (tempFile == NULL) {
        perror("Error creating temporary file");
        return ;
    }
    // Get the file descriptor associated with the FILE*
    input_fd = fileno(tempFile);
	if ( input_fd == -1 )
		perror("tmpfile");
	int bytes;
	std::cout << request.body.size() << std::endl;
	for ( size_t i = 0; i < request.body.size(); i++ )
	{
		bytes = write( input_fd, &request.body[ i ], sizeof( char ) );
		if ( bytes == -1 )
		{
			perror("write");
			return ;
		}
		//std::cout << request.body[ i ];
	}
	if (lseek(input_fd, SEEK_SET, 0) == -1) {
        perror("fcntl");
        exit(1);
    }
}

void Cgi::setup( const Request & request, const Response & response )
{
	( void ) request;
	std::cout << "entered cgi\n";
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
			perror( "indup" );
		if ( dup2( fd[ 1 ], 1 ) == -1 )
			perror( "outdup" );
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
}

void Cgi::clear()
{
	body.clear();
}

