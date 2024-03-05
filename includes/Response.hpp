#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <vector>
# include <sstream>
# include "Request.hpp"

class Response
{
	public:
		std::string protocol;
		int statusCode;
		std::string status;
		std::vector<std::pair<std::string, std::string> > header;
		std::vector<int> body;
		ServerConfig serverConfig;
		Location location;
		Response();
		Response( const Response & response );
		Response & operator=( const Response & response );
		~Response();
		void setup( const Request & request, const ServerInfo & serverInfo );
		void setServerConfig( const Request & request, const ServerInfo & serverInfo );
		void response();
		std::string resource;
		//bool cgi;
		int locationIndex;
		//int resourceType;
		bool is_url_found_in_path( const std::string & requestPath, const std::string & locationPath  );
		std::pair<std::string, int> getFinalLocationPath( const std::vector<std::pair<std::string, int> > & locationFound );
		void urlHandle( std::string requestPath );
		void redirectionHandle();
		void methodHandle( const Request & request );
		//void resourceHandle(const char * path);
		/*bool index_exist();
		void fill_body();
		bool has_trailing_slach();
		void handle_directory( const Request & request, const ServerConfig & serverConfig );
		void handle_file();
		void handle_get( const Request & request, const ServerConfig & serverConfig );
		void handle_post();
		void launch( const Request & request, const ServerConfig & serverConfig );
		void clear();
		bool is_cgi();
		void handle_cgi();*/
		//void get_resource();
		//void check_method( const std::string & method, const std::vector<std::string> & methods );
		//bool is_found( const std::string & requestPath, const std::string & locationPath );
};

#endif
