/**
 * @file Date.hxx
 * @brief Declares the Date class to handle UNIX timestamps conversions.
 */

#pragma once

/**
 * @class Date
 * @brief A utility class for handling date-related functions.
 *
 * The `Date` class provides functionality for converting Unix timestamps into human-readable date and time formats.
 * It supports converting timestamps into local time strings, making it useful for applications that deal with
 * time-based data.
 */
class Date
{
  public:
    /**
     * @brief Converts a Unix timestamp into a human-readable local date and time string.
     *
     * This function converts a Unix timestamp (seconds since January 1, 1970) into a string formatted as:
     * "YYYY/MM/DD HH:MM AM/PM". It returns "Never" if the timestamp is less than or equal to zero,
     * "Invalid timestamp" if the timestamp cannot be converted, and "Formatting error" if the string formatting fails.
     *
     * @param unix_timestamp The Unix timestamp to convert, in seconds since January 1, 1970.
     * @return A human-readable string representing the local date and time corresponding to the given timestamp.
     */
    static std::string toLocaleString(long long int unix_timestamp);
};
