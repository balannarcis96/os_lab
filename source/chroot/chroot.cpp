#include "chroot.h"

std::string GTargetRoot = "";

bool ProcessArguments( int argc, char **argv ) noexcept
{
	ArgmutentsParser Parser( argc, argv );

	GTargetRoot = Parser.GetFileName( );
	if( GTargetRoot.empty( ) )
	{
		puts( "chroot: missing operand\nTry 'chroot -h' for more information.\n" );
		return false;
	}

	return true;
}

bool ProcessInput( ) noexcept
{
	if( !DirectoryExists( GTargetRoot.c_str( ) ) )
	{
		printf( "chroot: cannot change root directory to '%s': No such file or "
				"directory\n",
				GTargetRoot.c_str( ) );
		return false;
	}

	if( chdir( GTargetRoot.c_str( ) ) != 0 )
	{
		return false;
	}

	char Buffer[ PATH_MAX ]{ 0 };

	if( getcwd( Buffer, PATH_MAX ) )
	{
		printf( "chroot: current working dirrectory is: %s\n", Buffer );
	}

	return true;
}

int main_chroot( int argc, char **argv ) noexcept
{
	const auto ProcessArgumentsResult = ProcessArguments( argc, argv );
	if( !ProcessArgumentsResult )
	{
		return ERROR_FAILED_TO_PARSE_ARGUMENTS;
	}

	if( !ProcessInput( ) )
	{
		return FAIL;
	}

	return SUCCESS;
}