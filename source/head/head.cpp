#include "head.h"

static int		   GNumberOfBytes  = 23;
static int		   GNumberOfLines  = 10;
static bool		   GQuite		   = true;
static std::string GTargetFileName = "";

static bool ProcessArguments( int argc, char **argv ) noexcept
{
	ArgmutentsParser Parser( argc, argv );

	auto Option = Parser.FindWithValue( "-c" );
	if( !Option.first.empty( ) )
	{
		if( Option.second.empty( ) )
		{
			puts( "head: Invalid -c usage, see usage string (-h)!\n" );
			return false;
		}

		if( !IsIntegerString( Option.second.c_str( ) ) )
		{
			printf( "head: Invalid -c usage! Invalid number of bytes [%s]! See usage string (-h)!\n", Option.second.c_str( ) );
			return false;
		}

		GNumberOfBytes = atoi( Option.second.c_str( ) );
	}

	Option = Parser.FindWithValue( "-n" );
	if( !Option.first.empty( ) )
	{
		if( Option.second.empty( ) )
		{
			puts( "head: Invalid -n usage, see usage string (-h)!\n" );
			return false;
		}

		if( !IsIntegerString( Option.second.c_str( ) ) )
		{
			printf( "head: Invalid -n usage! Invalid number of lines [%s]! See usage string (-h)!\n", Option.second.c_str( ) );
			return false;
		}

		GNumberOfLines = atoi( Option.second.c_str( ) );
	}

	if( Parser.Exists( "-q" ) )
	{
		GQuite = true;
	}

	if( Parser.Exists( "-v" ) )
	{
		GQuite = false;
	}

	GTargetFileName = Parser.GetFileName( );

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