#include "webserv.hpp"

int	main( int argc, char **argv )
{
	std::string file;
	Http http;

	if ( argc > 2 )
		return ( 1 );
	file = argv[1] ? argv[1] : "config_file";
	http.execute( file );
	//http.setup(file);
	//http.launch();
	//maybe do an http exec
}
