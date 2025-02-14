#include "Tools/Logger.hxx"
#include "stdafx.hxx"

Logger::Logger(const std::string logger) : logger(logger)
{}

Logger::~Logger()
{}

void Logger::logInfo(const std::string &log)
{
    TerminalColor::set(BrightBlue);

    std::cerr << "[ " << logger << " ] -> INFO: " << log << "\n";

    TerminalColor::reset();
}

void Logger::logWarning(const std::string &log)
{
    TerminalColor::set(Yellow);

    std::cout << "[ " << logger << " ] -> WARNING: " << log << "\n";

    TerminalColor::reset();
}

void Logger::logError(const std::string &log, const bool &throw_runtime_err)
{
    TerminalColor::set(Red);

    std::cerr << "[ " << logger << " ] -> ERROR: " << log << "\n";

    if (throw_runtime_err)
        throw std::runtime_error("[ " + logger + " ] -> ERROR: " + log + "\n");

    TerminalColor::reset();
}
