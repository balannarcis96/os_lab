#pragma once

bool HasTerminalStandardIn( ) noexcept;

bool IsIntegerString( const char *Str ) noexcept;

class ArgmutentsParser
{
public:
	ArgmutentsParser( int argc, char **argv ) noexcept
	{
		for( int i = 1; i < argc; ++i )
		{
			Parts.push_back( argv[ i ] );
		}
	}

	const std::string &Find( const char *Name ) const noexcept
	{
		auto Result = std::find( Parts.begin( ), Parts.end( ), std::string( Name ) );
		if( Result != Parts.end( ) )
		{
			return *Result;
		}

		return Empty;
	}

	const std::pair< const std::string &, std::ptrdiff_t > FindStartsWith( const char *StartsWithName ) const noexcept
	{
		auto Result = std::find_if( Parts.begin( ), Parts.end( ), [ StartsWithName ]( const std::string &Item ) noexcept { return Item.rfind( StartsWithName, 0 ) == 0; } );
		if( Result != Parts.end( ) )
		{
			return { *Result, Result - Parts.begin( ) };
		}

		return { Empty, 0 };
	}

	std::pair< std::string, std::string > FindWithValue( const char *Name ) const noexcept
	{
		const auto Delimiter = "=";

		auto Result = FindStartsWith( Name );
		if( Result.first.empty( ) )
		{
			return { "", "" };
		}

		const auto DelimiterIndex = Result.first.find( Delimiter );
		if( DelimiterIndex == std::string::npos )
		{
			// Treat the next argument as the value if possible
			if( Result.second + 1 >= Parts.size( ) )
			{
				return { Result.first, "" };
			}

			return { Result.first, Parts[ Result.second + 1 ] };
		}

		const auto Key	 = Result.first.substr( 0, DelimiterIndex );
		const auto Value = Result.first.substr( DelimiterIndex + 1, Result.first.size( ) - 1 );

		return { Key, Value };
	}

	bool Exists( const char *Name ) const noexcept
	{
		return std::find( Parts.begin( ), Parts.end( ), std::string( Name ) ) != Parts.end( );
	}

	const std::string &GetFileName( ) const noexcept
	{
		if( Parts.empty( ) )
		{
			return Empty;
		}

		for( size_t i = Parts.size( ) - 1; i >= 0; --i )
		{
			const auto &Item = Parts[ i ];
			if( !Item.empty( ) && Item[ 0 ] != '-' )
			{
				return Item;
			}
		}

		return Empty;
	}

	const std::vector< std::string > &GetParts( ) const noexcept
	{
		return Parts;
	}

private:
	static const std::string Empty;

	std::vector< std::string > Parts;
};

bool DirectoryExists( const char *pzPath ) noexcept;

std::string Trim( std::string Source ) noexcept;

std::vector< std::string > Split( const std::string &Source, char Delimtier ) noexcept;

std::vector< std::string > SplitAndTrim( const std::string &Source, char Delimtier ) noexcept;

std::string PerformChroot( std::string ChrootTarget ) noexcept;