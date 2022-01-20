#pragma once

#include <shared.h>

class Command
{
public:
	Command( ) noexcept = default;

	~Command( ) noexcept
	{
		Clear( );
	}

	void SetCommandParts( const std::vector< std::string > &InCommandParts ) noexcept
	{
		CommandParts = InCommandParts;
	}

	std::string ToString( ) const noexcept
	{
		std::string Temp;
		for( size_t i = 0; i < CommandParts.size( ); ++i )
		{
			Temp += CommandParts[ i ];

			if( CommandParts.size( ) > 1 && i < CommandParts.size( ) - 1 )
			{
				Temp += " ";
			}
		}

		return Temp;
	}

	std::string ToStringArguments( ) const noexcept
	{
		std::string Temp;
		for( size_t i = 0; i < CommandParts.size( ); ++i )
		{
			Temp += CommandParts[ i ];

			if( CommandParts.size( ) > 1 && i < CommandParts.size( ) - 1 )
			{
				Temp += " ";
			}
		}

		return Temp;
	}

	std::string GetCommandName( ) const noexcept
	{
		if( CommandParts.empty( ) )
		{
			return "";
		}

		return CommandParts[ 0 ];
	}

	void Clear( ) noexcept
	{
		auto Temp = Argv;
		if( Temp )
		{
			while( *Temp )
			{
				free( Temp );
				Temp++;
			}

			free( Argv );
			Argv = nullptr;
			Argc = 0;
		}
	}

	char **GetArgv( ) noexcept
	{
		return Argv;
	}

	int GetArgc( ) const noexcept
	{
		return Argc;
	}

	bool IsValid( ) const noexcept
	{
		return Argc > 0 && Argv;
	}

	int Execve( ) noexcept;

	int Execvp( ) noexcept;

	int ExecvpShell( ) noexcept;

	std::vector< std::string > CommandParts;

private:
	void FromStringList( const std::vector< std::string > &StringList ) noexcept;

	char **Argv = nullptr;
	int	   Argc = 0;

	friend class CommandOrConnector;
};

class CommandOrConnector
{
public:
	std::string Value = "";
	Command		ParsedCommand;
	std::string ChrootTarget;
	bool		bIsCommand	= false;
	bool		bIsChroot	= false;
	bool		IsFile		= false;
	bool		ShowHelp	= false;
	bool		ShowVersion = false;

	bool operator==( const CommandOrConnector &Other ) const noexcept
	{
		return Value == Other.Value && bIsCommand == Other.bIsCommand;
	}

	bool IsEmpty( ) const noexcept
	{
		return *this == Empty || Value.empty( );
	}

	bool IsConnector( ) const noexcept
	{
		return !IsEmpty( ) && !bIsCommand;
	}

	bool IsChroot( ) const noexcept
	{
		return !IsEmpty( ) && bIsChroot;
	}

	bool IsCommand( ) const noexcept
	{
		return !IsEmpty( ) && bIsCommand;
	}

	void PrepareCustomCommand( ) noexcept;

	static const CommandOrConnector Empty;
};

class CommandLineParser
{
public:
	bool ParseCommand( const std::string &Command ) noexcept;

	std::vector< CommandOrConnector > &GetResult( ) noexcept
	{
		return Result;
	}

	void Clear( ) noexcept
	{
		Result.clear( );
	}

private:
	std::vector< CommandOrConnector > Result;
};
