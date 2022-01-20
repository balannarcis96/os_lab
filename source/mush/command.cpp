#include "command.h"

#include "commandParser.h"
#include "pipe.h"

const char *   C_Command_NL_Path			  = "./commands/nl";
const char *   C_Command_HEAD_Path			  = "./commands/head";
const char *   C_Command_CHROOT_Path		  = "./commands/chroot";
constexpr auto CBufferSize					  = 1024 * 16;
char		   GTransferBuffer[ CBufferSize ] = { 0 };

extern char **environ;

int DoCommand( CommandOrConnector &Command, const CommandOrConnector &Prev, const CommandOrConnector &Next ) noexcept
{
	int Pipe_StdIn[ 2 ];
	int Pipe_StdOut[ 2 ];

	if( Prev.IsConnector( ) || Prev.IsFile )
	{
		if( pipe( Pipe_StdIn ) == -1 )
		{
			printf( "mush: Failed to open pipe!\n" );
			return FAIL;
		}
	}

	if( Next.IsConnector( ) )
	{
		if( pipe( Pipe_StdOut ) == -1 )
		{
			printf( "mush: Failed to open pipe!\n" );
			return FAIL;
		}
	}

	const auto ForkResult_PID = fork( );
	if( ForkResult_PID == 0 )
	{
		if( Prev.IsChroot( ) )
		{
			const std::string &TargetRoot = Prev.ChrootTarget;
			if( !DirectoryExists( TargetRoot.c_str( ) ) )
			{
				printf( "chroot: cannot change root directory to '%s': No such directory\n", TargetRoot.c_str( ) );
				exit( FAIL );
			}

			if( chdir( TargetRoot.c_str( ) ) != 0 )
			{
				printf( "chroot: cannot change root directory to '%s'!\n", TargetRoot.c_str( ) );
				exit( FAIL );
			}

			printf( "chroot: set cwd: %s\n", TargetRoot.c_str( ) );
		}

		if( Prev.IsConnector( ) && Prev.Value == ">" )
		{
			if( !Command.IsFile )
			{
				printf( "mush: > must be predecessed by a command!\n" );
				exit( FAIL );
			}

			close( Pipe_StdIn[ 1 ] );

			if( !Pipe::ReadFromPipe( Pipe_StdIn[ 0 ], GTransferBuffer, CBufferSize ) )
			{
				exit( ERROR_FAILED_TO_READ_FROM_PIPE );
			}

			close( Pipe_StdIn[ 0 ] );

			const auto StringSize = strnlen( GTransferBuffer, CBufferSize );
			if( StringSize == CBufferSize )
			{
				exit( ERROR_FAILED_TO_READ_FROM_PIPE );
			}

			std::ofstream File = std::ofstream( Command.Value.c_str( ) );
			if( !File.is_open( ) )
			{
				exit( ERROR_FAILED_TO_OPEN_FILE );
			}

			File.write( GTransferBuffer, StringSize );

			File.close( );

			exit( SUCCESS );
		}

		if( Prev.IsConnector( ) || Prev.IsFile )
		{
			if( dup2( Pipe_StdIn[ 0 ], STDIN_FILENO ) == -1 )
			{
				exit( FAIL );
			}

			close( Pipe_StdIn[ 0 ] );
			close( Pipe_StdIn[ 1 ] );
		}

		if( Next.IsConnector( ) )
		{
			if( dup2( Pipe_StdOut[ 1 ], STDOUT_FILENO ) == -1 )
			{
				exit( errno );
			}

			close( Pipe_StdOut[ 0 ] );
			close( Pipe_StdOut[ 1 ] );
		}

		int ExecuteResult = SUCCESS;
		if( Command.ParsedCommand.GetCommandName( ) == "nl" )
		{
			Command.PrepareCustomCommand( );
			ExecuteResult = main_nl( Command.ParsedCommand.GetArgc( ), Command.ParsedCommand.GetArgv( ) );
		}
		else if( Command.ParsedCommand.GetCommandName( ) == "head" )
		{
			Command.PrepareCustomCommand( );
			ExecuteResult = main_head( Command.ParsedCommand.GetArgc( ), Command.ParsedCommand.GetArgv( ) );
		}
		else
		{
			ExecuteResult = Command.ParsedCommand.Execvp( );
		}

		exit( ExecuteResult );
	}
	else
	{
		if( Prev.IsConnector( ) || Prev.IsFile )
		{
			close( Pipe_StdIn[ 0 ] );

			const auto WriteLen = strnlen( GTransferBuffer, CBufferSize );
			if( WriteLen == CBufferSize )
			{
				printf( "mush: Write to pipe error!\n" );
				return -1;
			}

			Pipe::WriteToPipe( Pipe_StdIn[ 1 ], GTransferBuffer, WriteLen + 1 );

			close( Pipe_StdIn[ 1 ] );
		}

		if( Next.IsConnector( ) )
		{
			close( Pipe_StdOut[ 1 ] );
			Pipe::ReadFromPipe( Pipe_StdOut[ 0 ], GTransferBuffer, CBufferSize );

			close( Pipe_StdOut[ 0 ] );
		}

		int CommandResult;
		waitpid( ForkResult_PID, &CommandResult, 0 );

		return CommandResult;
	}

	return FAIL;
}

void ProcessCommandChain( std::vector< CommandOrConnector > &Command ) noexcept
{
	for( size_t i = 0; i < Command.size( ); ++i )
	{
		auto &Part = Command[ i ];
		if( Part.IsCommand( ) )
		{
			const auto &Prev = i == 0 ? CommandOrConnector::Empty : Command[ i - 1 ];
			const auto &Next = ( i + 1 ) < Command.size( ) ? Command[ i + 1 ] : CommandOrConnector::Empty;

			if( Prev.IsConnector( ) && Prev.Value == ">" )
			{
				Part.IsFile = true;
			}

			if( Part.ParsedCommand.GetCommandName( ) == "chroot" )
			{
				if( Part.ShowHelp )
				{
					puts( "Balan Narcis[chroot]\n"
						  "Usage: chroot NEWROOT [COMMAND [ARG]...]\n"
						  "Run COMMAND with root directory set to NEWROOT.\n" );
				}

				if( Part.ShowVersion )
				{
					puts( "chroot (Balan Narcis) 1.0\n" );
				}

				//printf( "mush: chroot skipped [target=%s]\n", Part.ChrootTarget.c_str( ) );
				continue;
			}

			const auto Result = DoCommand( Part, Prev, Next );
			if( Result != SUCCESS )
			{
				std::cout << Part.ParsedCommand.GetCommandName( ) << ": Existed with status " << Result << std::endl;
				break;
			}
		}
	}
}

void ProcessCommand( const std::string &Command ) noexcept
{
	if( Command.empty( ) )
	{
		printf( "mush: No command given!\n" );
		return;
	}

	CommandLineParser CMDParser;
	if( CMDParser.ParseCommand( Command ) )
	{
		ProcessCommandChain( CMDParser.GetResult( ) );
	}
}
