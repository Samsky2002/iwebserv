#ifndef CGI_HPP
# define CGI_HPP

class Response;

class Cgi
{
	public:
		int fd[ 2 ];
		int input_fd;
		int pid;
		char *path;
		char **argv;
		char **env;
		char rbuff[ 10 ];
		int bytesRead;
		std::string body;
		void setup( const Request & request, const Response & response );
		void launch();
		void setupArgv( const Response & response );
		void setupEnv();
		void setupInputFile( const Request & request );
		void clear();
};

#endif
