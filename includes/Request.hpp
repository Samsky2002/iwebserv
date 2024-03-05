#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <vector>

class Request
{
	public:
		int statusCode;
		std::string method;
		std::string path;
		std::string protocol;
		std::vector<std::pair<std::string, std::string> > header;
		std::string body;
		// the body maybe should be something other than string
		Request();
		Request( const Request & request );
		Request & operator=( const Request & request );
		~Request();
		void setup( const std::string & str );
	/*	void fill_method_path_protocol( std::stringstream & ss );
		void check_method_path_protocol();
		void parse_method_path_protocol( const std::string & str );
		void parse_header( const std::string & str );
		void parse_request_body( const std::string & str );
		void parse_request_header( const std::string & str );
		void parse_request( const std::string & str );
		std::string get_key( const std::string & str );
		std::string get_value( const std::string & str );
		bool isUpperCase( const std::string & str );
		void check_header_info();
		bool key_exist( const std::string & key );
		//void check_header_info( const Request & request );
		void clear();
		void print();*/
};

#endif
