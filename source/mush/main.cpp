#include "mush.h"

std::string GCWD;

bool UpdateCWD() noexcept
{
    char Buffer[PATH_MAX]{0};

    if (!getcwd(Buffer, PATH_MAX))
    {
        return false;
    }

    GCWD = Buffer;

    return true;
}

bool PrintLineStart() noexcept
{
    if (!UpdateCWD())
    {
        return false;
    }

    std::cout << ">" << GCWD << "$ ";

    return true;
}

int ProcessCommand(const std::string &Command) noexcept
{
    return SUCCESS;
}

int main(int argc, char **argv)
{
    while (true)
    {
        if (!PrintLineStart())
        {
            return FAIL;
        }

        std::string Line;
        if (!std::getline(std::cin, Line))
        {
            break;
        }

        const int Result = ProcessCommand(Line);
        if (Result)
        {
            return Result;
        }
    }

    return SUCCESS;
}