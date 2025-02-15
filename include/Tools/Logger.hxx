/**
 * @file Logger.hxx
 * @brief Declares the Logger class to create a well-defined interface to display terminal messages and errors.
 */

#pragma once

#include "Tools/TerminalColor.hxx"
#include "Engine/Languages.hxx"

/**
 * @brief A logger class for logging messages with different severity levels.
 *
 * This class provides methods for logging informational messages, warnings, and errors. Each message
 * is prefixed with the logger's name and includes the severity level (INFO, WARNING, or ERROR). Errors can
 * optionally throw a runtime error exception or be printed to the standard error output in a colored format.
 */
class Logger
{
  private:
    std::string logger; ///< The name of the logger (who is logging), used as a prefix in the log messages.

  public:
    /**
     * @brief Constructs a Logger with a given name.
     *
     * The logger's name is used as a prefix in log messages to identify which logger generated the message.
     *
     * @param logger The name of the logger.
     */
    Logger(const std::string logger);

    /**
     * @brief Destructor for the Logger class.
     *
     * The destructor does not perform any specific actions for this class.
     */
    ~Logger();

    /**
     * @brief Logs an informational message.
     *
     * Logs a message at the INFO level, prefixed with the logger's name and printed to the standard error
     * output in bright blue color.
     *
     * @param log The message to log.
     */
    void logInfo(const std::string &log);

    /**
     * @brief Logs a warning message.
     *
     * Logs a message at the WARNING level, prefixed with the logger's name and printed to the standard output
     * in yellow color.
     *
     * @param log The message to log.
     */
    void logWarning(const std::string &log);

    /**
     * @brief Logs an error message.
     *
     * Logs a message at the ERROR level, prefixed with the logger's name. The message is printed to the standard
     * error output in red color. Optionally, throws a runtime error exception with the message.
     *
     * @param log The error message to log.
     * @param throw_runtime_err Flag indicating whether to throw a runtime error exception (default is true).
     */
    void logError(const std::string &log, const bool &throw_runtime_err = true);
};
