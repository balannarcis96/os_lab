#include "command.h"

#include "commandParser.h"
#include "pipe.h"

const char *   C_Command_NL_Path			  = "./commands/nl";
const char *   C_Command_HEAD_Path			  = "./commands/head";
const char *   C_Command_CHROOT_Path		  = "./commands/chroot";
constexpr auto CBufferSize					  = 1024 * 16;
char		   GTransferBuffer[ CBufferSize ] = { 0 };
bool		   GChrootEnabled				  = false;
std::string	   GChrootTarget;

extern char **environ;
extern bool	  GVerbose;
extern bool	  GUseCustomNL;
extern bool	  GUseCustomHead;

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

		if( GVerbose )
		{
			puts( "mush: Opened Parent -> Child write pipe!" );
		}
	}

	if( Next.IsConnector( ) )
	{
		if( pipe( Pipe_StdOut ) == -1 )
		{
			printf( "mush: Failed to open pipe!\n" );
			return FAIL;
		}

		if( GVerbose )
		{
			puts( "mush: Opened Parent <- Child read pipe!" );
		}
	}

	const auto ForkResult_PID = fork( );
	if( ForkResult_PID == 0 )
	{
		if( GVerbose )
		{
			if( ( Prev.IsConnector( ) || Prev.IsFile ) && !Next.IsEmpty( ) )
			{
				puts( "mush[Child]: plugged Parent -> Child -> stdin" );
			}

			if( Next.IsConnector( ) )
			{
				puts( "mush[Child]: plugged Parent <- Child <- stdout" );
			}

			if( GChrootEnabled )
			{
				printf( "mush[Child]: set cwd to: %s\n", GChrootTarget.c_str( ) );
			}

			if( Prev.IsConnector( ) && Prev.Value == ">" )
			{
				printf( "mush[Child]: [>] wrote to file \n\n" );
			}
			else
			{
				printf( "mush[Child][Executing]: [Prev]: %s [Command]: %s [Next]:%s\n\n", Prev.Value.c_str( ), Command.ParsedCommand.ToString( ).c_str( ), Next.Value.c_str( ) );
			}
		}

		if( GChrootEnabled )
		{
			const std::string &TargetRoot = GChrootTarget;
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
		else if( Prev.IsConnector( ) || Prev.IsFile )
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
		if( Command.ParsedCommand.GetCommandName( ) == "nl" && GUseCustomNL )
		{
			Command.PrepareCustomCommand( );
			ExecuteResult = main_nl( Command.ParsedCommand.GetArgc( ), Command.ParsedCommand.GetArgv( ) );
		}
		else if( Command.ParsedCommand.GetCommandName( ) == "head" && GUseCustomHead )
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

				GChrootEnabled = true;

				GChrootTarget = PerformChroot( Part.ChrootTarget );
				if( GChrootTarget.empty( ) )
				{
					return;
				}

				continue;
			}
			else if( Part.ParsedCommand.GetCommandName( ) == "cd" )
			{
				if( Part.ParsedCommand.CommandParts.size( ) < 2 )
				{
					puts( "mush: please provide the target directory for the cd command!\n" );
					return;
				}

				const std::string &TargetRoot = Part.ParsedCommand.CommandParts[ 1 ];
				if( !DirectoryExists( TargetRoot.c_str( ) ) )
				{
					printf( "cd: cannot change root directory to '%s': No such directory\n", TargetRoot.c_str( ) );
					exit( FAIL );
				}

				if( chdir( TargetRoot.c_str( ) ) != 0 )
				{
					printf( "cd: cannot change root directory to '%s'!\n", TargetRoot.c_str( ) );
					exit( FAIL );
				}

				return;
			}
			else if( Part.ParsedCommand.GetCommandName( ) == "verbose" )
			{
				if( Part.ParsedCommand.CommandParts.size( ) < 2 )
				{
					if( GVerbose )
					{
						puts( "mush: verbose enabled!\n\tUse verbose --disable to disable verbose output\n" );
					}
					else
					{
						puts( "mush: verbose disabled!\n\tUse verbose --enable to enable verbose output\n" );
					}
					return;
				}

				if( Part.ParsedCommand.CommandParts[ 1 ] == "--enable" )
				{
					GVerbose = true;
					puts( "mush: verbose enabled!\n" );
				}
				else if( Part.ParsedCommand.CommandParts[ 1 ] == "--disable" )
				{
					GVerbose = false;
					puts( "mush: verbose disabled!\n" );
				}
				else
				{
					puts( "mush: please use [verbose --enable/--disable]!\n" );
				}

				return;
			}
			else if( Part.ParsedCommand.GetCommandName( ) == "mush-nl" )
			{
				if( Part.ParsedCommand.CommandParts.size( ) < 2 )
				{
					if( GUseCustomNL )
					{
						puts( "mush: mush-nl[enabled] - using custom [nl] command!\n\tUse mush-nl --enable/--disable to switch between the custom and the system command\n" );
					}
					else
					{
						puts( "mush: mush-nl[disabled] - using system [nl] command!\n\tUse mush-nl --enable/--disable to switch between the custom and the system command\n" );
					}

					return;
				}

				if( Part.ParsedCommand.CommandParts[ 1 ] == "--enable" )
				{
					GUseCustomNL = true;
					puts( "mush: mush-nl[enabled] - using custom [nl] command!\n" );
				}
				else if( Part.ParsedCommand.CommandParts[ 1 ] == "--disable" )
				{
					GUseCustomNL = false;
					puts( "mush: mush-nl[disabled] - using system [nl] command!\n" );
				}
				else
				{
					puts( "mush: please use [mush-nl --enable/--disable]!\n" );
				}

				return;
			}
			else if( Part.ParsedCommand.GetCommandName( ) == "mush-head" )
			{
				if( Part.ParsedCommand.CommandParts.size( ) < 2 )
				{
					if( GUseCustomHead )
					{
						puts( "mush: mush-head[enabled] - using custom [head] command!\n\tUse mush-head --enable/--disable to switch between the custom and the system command\n" );
					}
					else
					{
						puts( "mush: mush-head[disabled] - using system [head] command!\n\tUse mush-head --enable/--disable to switch between the custom and the system command\n" );
					}

					return;
				}

				if( Part.ParsedCommand.CommandParts[ 1 ] == "--enable" )
				{
					GUseCustomHead = true;
					puts( "mush: mush-head[enabled] - using custom [head] command!\n" );
				}
				else if( Part.ParsedCommand.CommandParts[ 1 ] == "--disable" )
				{
					GUseCustomHead = false;
					puts( "mush: mush-head[disabled] - using system [head] command!\n" );
				}
				else
				{
					puts( "mush: please use [mush-head --enable/--disable]!\n" );
				}

				return;
			}
			else if( Part.ParsedCommand.GetCommandName( ) == "help" )
			{
				puts( "mush CLI (Balan Narcis)\n\n"
					  "Custom commands:\n"
					  "\tnl\n"
					  "\thead\n"
					  "\tchroot\n\n"
					  "Available commands:\n"
					  "\t[All available under /user/bin directory]\n\n"
					  "CLI commands:\n"
					  "\tmush-nl\t\t[OPTIONAL][--enable/--disable]\t\t-Switch between custom and system nl command\n"
					  "\tmush-head\t[OPTIONAL][--enable/--disable]\t\t-Switch between custom and system head command\n"
					  "\tverbose\t\t[OPTIONAL][--enable/--disable]\t\t-Enable or disable verbose output\n"
					  "\tclear\t\t\t\t\t\t\t-Clear the output buffer\n"
					  "\tcd\t\t[DIRECTORY]\t\t\t\t-Change the current directory\n" );
				return;
			}
			else if( Part.ParsedCommand.GetCommandName( ) == "version" )
			{
				puts( "mush CLI (Balan Narcis) v1.0\n" );
				return;
			}

			const auto Result = DoCommand( Part, Prev, Next );
			if( Result != SUCCESS )
			{
				//std::cout << Part.ParsedCommand.GetCommandName( ) << ": existed with status " << Result << std::endl;
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

	GChrootEnabled = false;

	CommandLineParser CMDParser;
	if( CMDParser.ParseCommand( Command ) )
	{
		ProcessCommandChain( CMDParser.GetResult( ) );
	}
}
