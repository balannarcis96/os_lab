#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <paths.h>
#include <signal.h>
#include <sstream>
#include <memory>
#include <fstream>
#include <ostream>

#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>

#include "utils.h"
#include "command.h"

enum Result : int
{
    SUCCESS = 0,
    FAIL,
    ERROR_FAILED_TO_PARSE_ARGUMENTS,
    ERROR_FILE_NOT_FOUND,
    ERROR_FAILED_TO_OPEN_FILE,
    ERROR_FAILED_TO_READ_FROM_PIPE
};
