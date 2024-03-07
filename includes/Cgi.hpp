#ifndef CGI_HPP
# define CGI_HPP

class Response;

class Cgi
{
	public:
		int fd[ 2 ];
		int pid;
		char *path;
		char **argv;
		char **env;
		char rbuff[ 10 ];
		int bytesRead;
		std::string body;
		void setup( const Response & response );
		void launch();
};

#endif
