#pragma once
#include <shared.h>

static const char* GNL_HelpString = 
"Narcis Balan [nl]\n"
"Usage: nl [OPTION]... [FILE]...\n"                                                             
"Write each FILE to standard output, with line numbers added.\n"
"With no FILE, or when FILE is -, read standard input.\n\n"
"  -d, --section-delimiter=CC      use CC for logical page delimiters\n"
"  -s, --number-separator=STRING   add STRING after (possible) line number\n"
"      --help     display this help and exit\n"
"      --version  output version information and exit\n";

int main_nl( int argc, char **argv ) noexcept;
