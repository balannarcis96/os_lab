#include "shared.h"

const std::string ArgmutentsParser::Empty = "";

bool HasTerminalStandardIn() noexcept { return isatty(STDIN_FILENO); }

bool IsIntegerString(const char *Str) noexcept {
  if (!Str || !Str[0]) {
    return false;
  }

  if (Str[0] == '-') {
    ++Str;
  }

  if (!Str[0]) {
    return false;
  }

  while (*Str) {
    if (!std::isdigit(*Str)) {
      return false;
    }

    ++Str;
  }

  return true;
}

bool DirectoryExists(const char *pzPath) noexcept {
  if (pzPath == NULL)
    return false;

  DIR *pDir;
  bool bExists = false;

  pDir = opendir(pzPath);

  if (pDir != NULL) {
    bExists = true;
    closedir(pDir);
  }

  return bExists;
}

std::vector<std::string> SplitAndTrim(std::string Source,
                                      char Delimtier) noexcept {}
