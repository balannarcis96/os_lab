#include "commandParser.h"

const CommandOrConnector CommandOrConnector::Empty;

void Command::FromStringList( const std::vector< std::string > &StringList ) noexcept
{
	Clear( );

	Argv = reinterpret_cast< char ** >( malloc( sizeof( char * ) * ( StringList.size( ) + 1 ) ) );
	memset( Argv, 0, sizeof( char * ) * ( StringList.size( ) + 1 ) );

	for( size_t i = 0; i < StringList.size( ); ++i )
	{
		const auto &String = StringList[ i ];

		Argv[ i ] = reinterpret_cast< char * >( malloc( sizeof( char ) * String.size( ) + 1 ) );
		memcpy( Argv[ i ], String.data( ), String.size( ) + 1 );
	}

	Argc = static_cast< int >( StringList.size( ) );
};

int Command::Execve( ) noexcept
{
	char CWD[ PATH_MAX ];
	getcwd( CWD, PATH_MAX );

	std::vector< std::string > FinalParts;
	FinalParts.push_back( CWD );

	bool bFirst = true;
	for( const auto &Part : CommandParts )
	{
		if( bFirst )
		{
			bFirst = false;
			continue;
		}

		FinalParts.push_back( Part );
	}

	FromStringList( FinalParts );

	for( const auto &Part : FinalParts )
	{
		std::cout << "FPart:" << Part << "\n";
	}

	std::string FinalCommand = "/usr/bin/";
	FinalCommand += GetCommandName( );

	std::cout << "FCommand: " << FinalCommand << "\n";

	return execve( FinalCommand.c_str( ), Argv, environ );
}

int Command::ExecvpShell( ) noexcept
{
	std::string FinalCommand;
	for( const auto &Part : CommandParts )
	{
		FinalCommand += Part + " ";
	}

	char FinalCommandBuffer[ 4096 ];
	strcpy( FinalCommandBuffer, FinalCommand.c_str( ) );

	char *const argv[] = {
		"sh",
		"-c",
		FinalCommandBuffer,
		nullptr
	};

	return execvp( argv[ 0 ], argv );
}

int Command::Execvp( ) noexcept
{
	FromStringList( CommandParts );

	return execvp( Argv[ 0 ], Argv );
}

bool CommandLineParser::ParseCommand( const std::string &Command ) noexcept
{
	Clear( );

	std::vector< std::string > TempList;
	std::string				   Temp;

	bool bInQutes = false;
	for( auto Char : Command )
	{
		if( Char == ' ' && !bInQutes )
		{
			Temp = Trim( Temp );
			if( Temp.empty( ) )
			{
				continue;
			}

			TempList.push_back( Temp );
			Temp = "";
		}
		else if( Char == '\"' )
		{
			if( bInQutes )
			{
				TempList.push_back( Temp );
				Temp = "";

				bInQutes = false;
			}
			else
			{
				bInQutes = true;
			}
		}
		else
		{
			Temp += Char;
		}
	}

	if( !Temp.empty( ) )
	{
		TempList.push_back( Temp );
	}

	std::vector< std::string > Reconstructed;

	for( const auto &Item : TempList )
	{
		if( Item == "|" || Item == ">" )
		{
			if( !Reconstructed.empty( ) )
			{
				CommandOrConnector NewCommand;
				NewCommand.Value	  = Reconstructed[ 0 ];
				NewCommand.bIsCommand = true;

				NewCommand.ParsedCommand.SetCommandParts( Reconstructed );

				Result.push_back( NewCommand );

				Reconstructed.clear( );
			}

			CommandOrConnector NewConnector;
			NewConnector.Value		= Item;
			NewConnector.bIsCommand = false;

			Result.push_back( NewConnector );
		}
		else
		{
			Reconstructed.push_back( Item );
		}
	}

	if( !Reconstructed.empty( ) )
	{
		CommandOrConnector NewCommand;
		NewCommand.Value	  = Reconstructed[ 0 ];
		NewCommand.bIsCommand = true;

		NewCommand.ParsedCommand.SetCommandParts( Reconstructed );

		Result.push_back( NewCommand );
	}

	if( !Result.empty( ) )
	{
		if( !Result[ 0 ].bIsCommand )
		{
			printf( "mush: command cannot start with connector!\n" );
			return false;
		}

		if( Result.size( ) > 1 )
		{
			if( !Result.back( ).bIsCommand )
			{
				printf( "mush: The command chain must end in a command or file name\n" );
				return false;
			}
		}

		bool bIsCommand = Result[ 0 ].bIsCommand;
		for( size_t i = 1; i < Result.size( ); ++i )
		{
			const auto &Item = Result[ i ];

			if( bIsCommand && Item.bIsCommand )
			{
				printf( "mush: Cannot have consecutive commands without a connector (| , >)\n" );
				return false;
			}
			else if( !bIsCommand && !Item.bIsCommand )
			{
				printf( "mush: Cannot have consecutive connectors without a command or filename\n" );
				return false;
			}

			bIsCommand = Item.bIsCommand;
		}
	}

	return !Result.empty( );
}

void CommandOrConnector::PrepareCustomCommand( ) noexcept
{
	ParsedCommand.FromStringList( ParsedCommand.CommandParts );
}