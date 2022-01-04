#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <stdio.h>

#include <dirent.h>

#include "utils.h"
#include "command.h"

enum Result : int
{
    SUCCESS = 0,
    FAIL,
    ERROR_FAILED_TO_PARSE_ARGUMENTS,
    ERROR_FILE_NOT_FOUND
};
