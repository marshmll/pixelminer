#include "Tools/Logger.hxx"
#include "stdafx.hxx"

Logger::Logger(const std::string logger) : logger(logger)
{}

Logger::~Logger() = default;

void Logger::logInfo(const std::string &log)
{
    TerminalColor::set(Green);

    std::cerr << "INFO";

    TerminalColor::reset();

    std::cerr << "    [ " << logger << " ] => " << log << "\n";
}

void Logger::logWarning(const std::string &log)
{
    TerminalColor::set(Yellow);

    std::cout << "WARNING";

    TerminalColor::reset();

    std::cout << " [ " << logger << " ] => " << log << "\n";
}

void Logger::logError(const std::string &log, const bool &throw_runtime_err)
{
    TerminalColor::set(Red);

    std::cerr << "ERROR";

    TerminalColor::reset();

    std::cerr << "   [ " << logger << " ] => " << log << "\n";

    if (throw_runtime_err)
        throw std::runtime_error("ERROR   [ " + logger + " ] => " + log);
}
