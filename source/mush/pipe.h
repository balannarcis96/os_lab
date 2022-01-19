#pragma once

#include <shared.h>

class Pipe
{
public:
	~Pipe( ) noexcept
	{
		Close( );
	}

	bool Open( ) noexcept;

	void Close( ) noexcept;

	void CloseRead( ) noexcept;

	void CloseWrite( ) noexcept;

	void DupWrite( int TargetFD ) noexcept;

	void DupRed( int TargetFD ) noexcept;

	int GetWrite( ) const noexcept
	{
		return PipeFD[ 1 ];
	}

	int GetRead( ) const noexcept
	{
		return PipeFD[ 0 ];
	}

	bool Write( const char *InBuffer, size_t InSize ) noexcept;

	bool Read( char *InBuffer, size_t InReadSize ) noexcept;

	static bool WriteToPipe( int Pipe, const char *InBuffer, size_t InSize ) noexcept;

	static bool ReadFromPipe( int Pipe, char *InBuffer, size_t InReadSize ) noexcept;

	bool IsOpen( ) const noexcept
	{
		return PipeFD[ 0 ] != 0 && PipeFD[ 1 ] != 0;
	}

private:
	int PipeFD[ 2 ] = { 0 };
};
