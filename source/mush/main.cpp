#include "mush.h"

std::string GCWD;

bool UpdateCWD( ) noexcept
{
	char Buffer[ PATH_MAX ]{ 0 };

	if( !getcwd( Buffer, PATH_MAX ) )
	{
		return false;
	}

	GCWD = Buffer;

	return true;
}

bool PrintLineStart( ) noexcept
{
	if( !UpdateCWD( ) )
	{
		return false;
	}

	std::cout << ">" << GCWD << "$ ";

	return true;
}

int main( int argc, char **argv )
{
	system( "clear" );

	printf( "############# MUSH-CLI ###############\n\n" );

	while( true )
	{
		if( !PrintLineStart( ) )
		{
			return FAIL;
		}

		std::string Line;
		if( !std::getline( std::cin, Line ) )
		{
			break;
		}

		if( Line == "clear" || Line == "cls" )
		{
			system( "clear" );
			continue;
		}

		ProcessCommand( Line );
	}

	return SUCCESS;
}
