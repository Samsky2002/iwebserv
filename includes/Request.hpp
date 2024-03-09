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
		std::vector<std::string> lines;	
		int index;
		std::string rem;
		bool inBody;
		size_t contentLength;
		// the body maybe should be something other than string
		Request();
		Request( const Request & request );
		Request & operator=( const Request & request );
		~Request();
		bool isUpperCaseLetters( const std::string & str );
		void handleMethod( const std::string & str );
		void handlePath( const std::string & str );
		void handleProtocol( const std::string & str );
		void handleRequestLine();
		std::string getKey( const std::string & str );
		std::string getValue( const std::string & str );
		void handleHeaders();
		void isEnd();
		void parse();
		void headerFill( const std::string & str );
		void bodyFill( const std::string & str );
		void setup( const std::string & str );
		// tmp
		int getContentLength();
	/*	void fill_method_path_protocol( std::stringstream & ss );
		void check_method_path_protocol();
		void parse_method_path_protocol( const std::string & str );
		void parse_header( const std::string & str );
		void parse_request_body( const std::string & str );
		void parse_request_header( const std::string & str );
		void parse_request( const std::string & str );
		void check_header_info();
		bool key_exist( const std::string & key );*/
		//void check_header_info( const Request & request );
		void clear();
		void print();
};

// path_info
// query_string
// 

#endif
