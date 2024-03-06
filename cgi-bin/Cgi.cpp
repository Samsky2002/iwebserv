class Cgi
{
	public:
		int fd[ 2 ];
		int pid;
		char *path;
		char *argv[];
		char *env[];
		void launch();
};

// he will need the body as an input file
// script path
// check extension of the script
// environment

void Cgi::setup()
{

}

void Cgi::launchCgi()
{
	if ( pipe( fd ) )
		std::cout << "pipe error\n";
	pid = fork();
	if ( pid == 0 )
	{
		if ( dup2(fd[ 1 ], 1) )
			perror( "dup2" );
		if ( execve( path, argv, env ) == -1 )
		{
			perror( "execve" );
			exit(1);
		}
	}
	wait( NULL );
}
