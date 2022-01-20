#include "nl.h"

static std::string GSectionDelimiter = "";
static std::string GNumberSeparator	 = "  ";
static std::string GTargetFileName	 = "";

static bool ProcessArguments( int argc, char **argv ) noexcept
{
	ArgmutentsParser Parser( argc, argv );

	if( Parser.Exists( "-h" ) || Parser.Exists( "--help" ) )
	{
		puts( GNL_HelpString );
		exit( SUCCESS );
	}

	if( Parser.Exists( "--version" ) )
	{
		puts( "nl (Balan narcis) 1.0!\n" );
		exit( SUCCESS );
	}

	auto Option = Parser.FindWithValue( "-d" );
	if( !Option.first.empty( ) )
	{
		if( Option.second.empty( ) )
		{
			puts( "nl: Invalid -d usage, see usage string (-h)!\n" );
			return false;
		}

		GSectionDelimiter = Option.second;
	}

	Option = Parser.FindWithValue( "-s" );
	if( !Option.first.empty( ) )
	{
		if( Option.second.empty( ) )
		{
			puts( "nl: Invalid -s usage, see usage string (-h)!\n" );
			return false;
		}

		GNumberSeparator = Option.second;
	}

	GTargetFileName = Parser.GetFileName( );

	return true;
}

static std::pair< std::vector< std::string >, int > GetInput( ) noexcept
{
	if( GTargetFileName.empty( ) )
	{
		printf( "nl: using stdin!\n" );

		int LineNumber = 1;
		for( std::string Line; std::getline( std::cin, Line ); )
		{
			std::cout << '\t' << LineNumber++ << GNumberSeparator << Line << "\n";
		}

		return { { }, SUCCESS };
	}

	//printf( "GetInput() -> %s\n", GTargetFileName.c_str( ) );

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
		return Lines.second;
	}

	int LineNumber = 1;
	for( const auto &Line : Lines.first )
	{
		std::cout << '\t' << LineNumber++ << GNumberSeparator << Line << "\n";
	}

	return SUCCESS;
}