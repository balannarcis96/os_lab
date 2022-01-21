#include "nl.h"

static std::string GSectionDelimiter = "";
static std::string GNumberSeparator	 = "  ";
static std::string GTargetFileName	 = "";

static int GHelpFlag	= 0;
static int GVersionFlag = 0;

static struct option GLongOptions[] = {
	{ "help", no_argument, &GHelpFlag, 1 },
	{ "version", no_argument, &GVersionFlag, 0 },
	{ "section-delimiter", no_argument, 0, 'd' },
	{ "number-separator", no_argument, 0, 's' },
	{ 0, 0, 0, 0 }
};

static bool ProcessArguments( int argc, char **argv ) noexcept
{
	int Opt		 = 0;
	int OptIndex = 0;
	while( 1 )
	{
		Opt = getopt_long( argc, argv, "d:fhs:v:", GLongOptions, &OptIndex );
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
						puts( GNL_HelpString );
						exit( SUCCESS );
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "version" ) )
					{
						puts( "nl (Balan narcis) 1.0!\n" );
						exit( SUCCESS );
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "section-delimiter" ) )
					{
						GSectionDelimiter = optarg;
					}
					else if( !strcmp( GLongOptions[ OptIndex ].name, "number-separator" ) )
					{
						GNumberSeparator = optarg;
					}
				}
			}
			break;
			case 'd':
			{
				GSectionDelimiter = optarg;
			}
			break;
			case 's':
			{
				GNumberSeparator = optarg;
			}
			break;
			case 'f':
			{
				GTargetFileName = optarg;
			}
			break;
			case '?':
			{
				printf( "nl: unknown parameter [%c], see help (--help)!\n\n", static_cast< char >( Opt ) );
			}
			break;
			default:
				printf( "nl: unknown parameter [%c], see help (--help)!\n\n", static_cast< char >( Opt ) );
				break;
		}
	}

	if( optind < argc )
	{
		GTargetFileName = argv[ optind ];
	}

	return true;
}

static std::pair< std::vector< std::string >, int > GetInput( ) noexcept
{
	if( GTargetFileName.empty( ) )
	{
		int LineNumber = 1;
		for( std::string Line; std::getline( std::cin, Line ); )
		{
			std::cout << '\t' << LineNumber++ << GNumberSeparator << Line << "\n";
		}

		return { { }, SUCCESS };
	}

	std::vector< std::string > Output;

	auto File = std::ifstream( GTargetFileName );
	if( !File.is_open( ) )
	{
		printf( "nl: ERROR_FILE_NOT_FOUND[%s]\n", GTargetFileName.c_str( ) );
		return { { }, ERROR_FILE_NOT_FOUND };
	}

	std::string Line;
	while( std::getline( File, Line ) )
	{
		Output.push_back( Line );
	}

	if( File.is_open( ) )
	{
		File.close( );
	}

	return { Output, SUCCESS };
}

int main_nl( int argc, char **argv ) noexcept
{
	const auto ProcessArgumentsResult = ProcessArguments( argc, argv );
	if( !ProcessArgumentsResult )
	{
		return ERROR_FAILED_TO_PARSE_ARGUMENTS;
	}

	const auto Lines = GetInput( );
	if( Lines.second != SUCCESS )
	{
		exit( Lines.second );
	}

	int LineNumber = 1;
	for( const auto &Line : Lines.first )
	{
		std::cout << '\t' << LineNumber++ << GNumberSeparator << Line << "\n";
	}

	return SUCCESS;
}