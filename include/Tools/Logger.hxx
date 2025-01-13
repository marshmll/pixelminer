#pragma once

class Logger
{
  private:
    std::string logger;

  public:
    Logger(const std::string logger);
    ~Logger();

    void logError(const std::string &log, const bool &throw_runtime_err = true);

    void logInfo(const std::string &log);
};