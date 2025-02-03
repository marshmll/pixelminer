#pragma once

#include "Tools/TerminalColor.hxx"

class Logger
{
  private:
    std::string logger;

  public:
    Logger(const std::string logger);
    ~Logger();

    void logInfo(const std::string &log);

    void logWarning(const std::string &log);

    void logError(const std::string &log, const bool &throw_runtime_err = true);

};