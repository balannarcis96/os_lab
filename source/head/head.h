#pragma once

#include <shared.h>

static const char* GHeadHelpString = 
"Balan Narcis[head]\n"
"Usage: head [OPTION]... [FILE]...\n"
"Print the first 10 lines of each FILE to standard output.\n"
"With more than one FILE, precede each with a header giving the file name.\n\n"
"With no FILE, or when FILE is -, read standard input.\n\n"
"Mandatory arguments to long options are mandatory for short options too.\n"
"  -c, --bytes=[-]NUM       print the first NUM bytes of each file;\n"
"                             with the leading '-', print all but the last\n"
"                             NUM bytes of each file\n"
"  -n, --lines=[-]NUM       print the first NUM lines instead of the first 10;\n"
"                             with the leading '-', print all but the last\n"
"                             NUM lines of each file\n"
"  -q, --quiet, --silent    never print headers giving file names\n"
"  -v, --verbose            always print headers giving file names\n"
"      --help     display this help and exit\n"
"      --version  output version information and exit\n";     

int main_head(int argc, char** argv) noexcept;
