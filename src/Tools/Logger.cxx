#include "Tools/Logger.hxx"
#include "stdafx.hxx"

Logger::Logger(const std::string logger) : logger(logger)
{}

Logger::~Logger() = default;

void Logger::logInfo(const std::string &log)
{
    TerminalColor::set(Green);

    std::cerr << _("INFO");

    TerminalColor::reset();

    std::cerr << " [" << logger << "] " << log << std::endl;
}

void Logger::logWarning(const std::string &log)
{
    TerminalColor::set(Yellow);

    std::cout << _("WARNING");

    TerminalColor::reset();

    std::cout << " [" << logger << "] " << log << std::endl;
}

void Logger::logError(const std::string &log, const bool &throw_runtime_err)
{
    TerminalColor::set(Red);

    std::cerr << _("ERROR");

    TerminalColor::reset();

    std::cerr << " [" << logger << "] " << log << std::endl;

    if (throw_runtime_err)
        throw std::runtime_error("[ " + logger + " ] => " + log);
}
