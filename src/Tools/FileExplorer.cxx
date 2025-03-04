#include "stdafx.hxx"
#include "Tools/FileExplorer.hxx"

void FileExplorer::browse(const std::string &folderPath)
{
#ifdef _WIN32
    // Windows: Use ShellExecute to open the folder in File Explorer
    ShellExecute(NULL, "open", folderPath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#elif __linux__
    // Linux: Use fork and exec to open the folder in the default file manager
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        execlp("xdg-open", "xdg-open", folderPath.c_str(), nullptr);
        // If execlp fails, print an error
        std::cerr << "Failed to open file explorer." << std::endl;
    }
    else if (pid > 0)
    {
        // Parent process: wait for the child to finish
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        // Fork failed
        std::cerr << "Failed to create a new process." << std::endl;
    }
#else
    // Unsupported platform
    std::cerr << "Unsupported operating system." << std::endl;
#endif
}
