#include "head.h"

static int		   GNumberOfBytes  = 9999;
static int		   GNumberOfLines  = 10;
static bool		   GQuite		   = true;
static std::string GTargetFileName = "";

static int GHelpFlag	= 0;
static int GVersionFlag = 0;
static int GVerbose = 0;

static struct option GLongOptions[] = {
	{ "help", no_argument, &GHelpFlag, 1 },
	{ "verbose", no_argument, &GVerbose, 'v' },
	{ "version", no_argument, &GVersionFlag, 0 },
	{ "bytes", required_argument, 0, 'c' },
	{ "lines", required_argument, 0, 'n' },
	{ "quiet", no_argument, 0, 'q' },
	{ 0, 0, 0, 0 }
};

static bool ProcessArguments( int argc, char **argv ) noexcept
{
	int Opt		 = 0;
	int OptIndex = 0;
	while( 1 )
	{
		Opt = getopt_long( argc, argv, "c:hn:qv", GLongOptions, &OptIndex );
		if( Opt == -1 )
		{
			break;
		}

		switch( Opt )
		{
			case 0:
			{
				if( GLongOptions[ OptIndex ].flag != 0 ) //help
				{
					if( !strcmp( GLongOptions[ OptIndex ].name, "help" ) )
					{
						puts( GHeadHelpString );
						exit( SUCCESS );
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "version" ) )
					{
						puts( "head (Balan narcis) 1.0!\n" );
						exit( SUCCESS );
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "bytes" ) )
					{
						GNumberOfBytes = atoi( optarg );
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "lines" ) )
					{
						GNumberOfLines = atoi( optarg );
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "quiet" ) )
					{
						GQuite = true;
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "verbose" ) )
					{
						GQuite = false;
					}
				}
			}
			break;
			case 'c':
			{
				GNumberOfBytes = atoi( optarg );
			}
			break;
			case 'n':
			{
				GNumberOfLines = atoi( optarg );
			}
			break;
			case 'v':
			{
				GQuite = false;
				GVerbose = true;
			}
			break;
			case '?':
			{
				printf( "head: unknown parameter [%c], see help (--help)!\n\n", static_cast< char >( Opt ) );
				return false;
			}
			default:
				printf( "head: unknown parameter [%c], see help (--help)!\n\n", static_cast< char >( Opt ) );
				return false;
		}
	}

	if( optind < argc )
	{
		GTargetFileName = argv[ optind ];
	}

	return true;
}

bool ProcessInput( const std::vector< std::string > &Lines ) noexcept
{
	if( !GQuite )
	{
		printf( "==> %s <==\n", GTargetFileName.c_str( ) );
	}

	if( GNumberOfBytes == 0 || GNumberOfLines == 0 )
	{
		return Lines.empty( );
	}

	const int TotalNumberOfLines = static_cast< int >( Lines.size( ) );

	int TotalNumOfBytes = 0;
	if( TotalNumberOfLines > 0 )
	{
		for( const auto &Line : Lines )
		{
			TotalNumOfBytes += static_cast< int >( Line.size( ) );
		}
		TotalNumOfBytes += TotalNumberOfLines - 1;
	}

	int NumOfBytes = GNumberOfBytes > 0 ? GNumberOfBytes : TotalNumOfBytes - GNumberOfBytes;
	int NumOfLines = GNumberOfLines > 0 ? GNumberOfLines : TotalNumberOfLines - GNumberOfLines;

	for( const auto &Line : Lines )
	{
		const auto LineSize = static_cast< int >( Line.size( ) );
		if( LineSize > NumOfBytes )
		{
			printf( "%s\n", Line.substr( 0, NumOfBytes ).c_str( ) );
			return false;
		}

		std::cout << Line << '\n';

		NumOfBytes -= LineSize + 1;
		--NumOfLines;

		if( !NumOfLines || !NumOfBytes )
		{
			return false;
		}
	}

	std::cout << '\n';

	return true;
}

static std::pair< std::vector< std::string >, int > GetInput( ) noexcept
{
	if( GTargetFileName.empty( ) )
	{
		int LineNumber = 1;
		for( std::string Line; std::getline( std::cin, Line ); )
		{
			if( !ProcessInput( { Line } ) )
			{
				break;
			}
		}

		return { { }, SUCCESS };
	}

	std::vector< std::string > Output;

	auto File = std::ifstream( GTargetFileName );
	if( !File.is_open( ) )
	{
		return { { }, ERROR_FILE_NOT_FOUND };
	}

	for( std::string Line; std::getline( File, Line ); )
	{
		Output.push_back( Line );
	}

	if( File.is_open( ) )
	{
		File.close( );
	}

	return { Output, SUCCESS };
}

int main_head( int argc, char **argv ) noexcept
{
	const auto ProcessArgumentsResult = ProcessArguments( argc, argv );
	if( !ProcessArgumentsResult )
	{
		return ERROR_FAILED_TO_PARSE_ARGUMENTS;
	}

	auto Input = GetInput( );
	if( Input.second != SUCCESS )
	{
		return Input.second;
	}

	if( !Input.first.empty( ) )
	{
		ProcessInput( Input.first );
	}

	return SUCCESS;
}