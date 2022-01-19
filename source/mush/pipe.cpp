#include "pipe.h"

bool Pipe::Open( ) noexcept
{
	const auto CreateResult = pipe( PipeFD );
	if( CreateResult == -1 )
	{
		return false;
	}

	return true;
}

void Pipe::Close( ) noexcept
{
	close( PipeFD[ 0 ] );
	close( PipeFD[ 1 ] );

	PipeFD[ 0 ] = 0;
	PipeFD[ 1 ] = 0;
}

void Pipe::CloseWrite( ) noexcept
{
	close( PipeFD[ 1 ] );

	PipeFD[ 1 ] = 0;
}

void Pipe::CloseRead( ) noexcept
{
	close( PipeFD[ 0 ] );

	PipeFD[ 0 ] = 0;
}

void Pipe::DupWrite( int TargetFD ) noexcept
{
	dup2( PipeFD[ 1 ], TargetFD );
}

void Pipe::DupRed( int TargetFD ) noexcept
{
	dup2( PipeFD[ 0 ], TargetFD );
}

bool Pipe::Write( const char *InBuffer, size_t InSize ) noexcept
{
	return WriteToPipe( PipeFD[ 1 ], InBuffer, InSize );
}

bool Pipe::Read( char *InBuffer, size_t InReadSize ) noexcept
{
	return ReadFromPipe( PipeFD[ 0 ], InBuffer, InReadSize );
}

bool Pipe::WriteToPipe( int Pipe, const char *InBuffer, size_t InSize ) noexcept
{
	ssize_t TotalWirtten = 0;
	do
	{
		const auto WrittenSize = write( Pipe, InBuffer + TotalWirtten, InSize );
		if( WrittenSize == -1 )
		{
			return false;
		}

		TotalWirtten += WrittenSize;
		InSize -= WrittenSize;
	} while( InSize );

	return true;
}

bool Pipe::ReadFromPipe( int Pipe, char *InBuffer, size_t InReadSize ) noexcept
{
	char Char;
	int	 Cursor = 0;
	while( read( Pipe, &Char, 1 ) == 1 )
	{
		if( Cursor == InReadSize - 2 )
		{
			InBuffer[ InReadSize - 1 ] = '\0';
			break;
		}

		InBuffer[ Cursor ] = Char;

		Cursor++;
	}

	return Cursor != 0;
}
