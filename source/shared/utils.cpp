#include "shared.h"

const std::string ArgmutentsParser::Empty = "";

bool HasTerminalStandardIn( ) noexcept
{
	return isatty( STDIN_FILENO );
}

bool IsIntegerString( const char *Str ) noexcept
{
	if( !Str || !Str[ 0 ] )
	{
		return false;
	}

	if( Str[ 0 ] == '-' )
	{
		++Str;
	}

	if( !Str[ 0 ] )
	{
		return false;
	}

	while( *Str )
	{
		if( !std::isdigit( *Str ) )
		{
			return false;
		}

		++Str;
	}

	return true;
}

bool DirectoryExists( const char *pzPath ) noexcept
{
	if( pzPath == NULL )
		return false;

	DIR *pDir;
	bool bExists = false;

	pDir = opendir( pzPath );

	if( pDir != NULL )
	{
		bExists = true;
		closedir( pDir );
	}

	return bExists;
}
std::string Trim( std::string Source ) noexcept
{
	//Left trim
	Source.erase( Source.begin( ), std::find_if( Source.begin( ), Source.end( ), std::not1( std::ptr_fun< int, int >( std::isspace ) ) ) );

	//Right trim
	Source.erase( std::find_if( Source.rbegin( ), Source.rend( ), std::not1( std::ptr_fun< int, int >( std::isspace ) ) ).base( ), Source.end( ) );

	return Source;
}

std::vector< std::string > Split( const std::string &Source, char Delimtier ) noexcept
{
	std::vector< std::string > Result;
	std::string				   Token;
	std::stringstream		   Stream( Source );

	while( std::getline( Stream, Token, Delimtier ) )
	{
		if( Token.empty( ) )
		{
			continue;
		}

		Result.push_back( Token );
	}

	std::cout << "Split: ";
	for( const auto &i : Result )
	{
		std::cout << "[" << i << "]";
	}
	std::cout << std::endl;

	return Result;
}

std::vector< std::string > SplitAndTrim( const std::string &Source, char Delimtier ) noexcept
{
	std::vector< std::string > Result;
	std::string				   Token;
	std::stringstream		   Stream( Source );

	while( std::getline( Stream, Token, Delimtier ) )
	{
		Token = Trim( Token );
		if( Token.empty( ) )
		{
			continue;
		}

		Result.push_back( Token );
	}

	std::cout << "SplitAndTrim: ";
	for( const auto &i : Result )
	{
		std::cout << "[" << i << "]";
	}
	std::cout << std::endl;

	return Result;
}

std::string PerformChroot( std::string ChrootTarget ) noexcept
{
	char Old[ PATH_MAX ];
	getcwd( Old, PATH_MAX );

	const std::string &TargetRoot = ChrootTarget;
	if( !DirectoryExists( TargetRoot.c_str( ) ) )
	{
		printf( "chroot: cannot change root directory to '%s': No such directory\n", TargetRoot.c_str( ) );
		return "";
	}

	if( chdir( TargetRoot.c_str( ) ) != 0 )
	{
		chdir( Old );
		printf( "chroot: cannot change root directory to '%s'!\n", TargetRoot.c_str( ) );
		return "";
	}

	char New[ PATH_MAX ];
	getcwd( New, PATH_MAX );

	chdir( Old );

	return New;
}
