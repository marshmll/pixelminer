#include "Tools/Logger.hxx"
#include "stdafx.hxx"

Logger::Logger(const std::string logger) : logger(logger)
{
}

Logger::~Logger()
{
}

void Logger::logError(const std::string &log, const bool &throw_runtime_err)
{
    if (throw_runtime_err)
        throw std::runtime_error("[ " + logger + " ] -> ERROR: " + log + "\n");
    else
        std::cerr << "[ " << logger << " ] -> ERROR: " << log << "\n";
}

void Logger::logInfo(const std::string &log)
{
    std::cerr << "[ " << logger << " ] -> INFO: " << log << "\n";
}