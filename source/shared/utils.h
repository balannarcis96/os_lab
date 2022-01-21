#pragma once

bool HasTerminalStandardIn( ) noexcept;

bool IsIntegerString( const char *Str ) noexcept;

bool DirectoryExists( const char *pzPath ) noexcept;

std::string Trim( std::string Source ) noexcept;

std::vector< std::string > Split( const std::string &Source, char Delimtier ) noexcept;

std::vector< std::string > SplitAndTrim( const std::string &Source, char Delimtier ) noexcept;

std::string PerformChroot( std::string ChrootTarget ) noexcept;