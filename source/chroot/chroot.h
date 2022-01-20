#pragma once

#include <shared.h>

static const char* GCHROOT_HelpString =
"Balan Narcis[chroot]\n"
"Usage: chroot NEWROOT [COMMAND [ARG]...]\n"
"Run COMMAND with root directory set to NEWROOT.\n";

int main_chroot(int argc, char** argv) noexcept;
