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
		std::vector<char> body;
		std::vector<std::string> lines;	
		int index;
		std::string rem;
		bool inBody;
		size_t contentLength;
		std::vector<char> buffer;
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
		void headerFill();
		void bodyFill();
		void setup( std::vector<char> & newBuffer );
		// tmp
		int getContentLength();
		/*bool key_exist( const std::string & key );*/
		//void check_header_info( const Request & request );
		void clear();
		void print();
};

// path_info
// query_string
// 

#endif
