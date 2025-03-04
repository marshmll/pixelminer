#pragma once

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#endif

class FileExplorer
{
  public:
    static void browse(const std::string &folder_path);
};
