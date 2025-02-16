#include "Tools/Date.hxx"
#include "stdafx.hxx"

std::string Date::toLocaleString(long long int unix_timestamp)
{
    if (unix_timestamp <= 0)
    {
        return _("Never");
    }

    // Convert the Unix timestamp to time_t
    std::time_t time = static_cast<std::time_t>(unix_timestamp);

    // Convert time_t to a tm struct
    std::tm *local_time = std::localtime(&time);

    // Check if local_time is valid
    if (!local_time)
    {
        return _("INVALID TIMESTAMP");
    }

    // Format the time into a readable string
    char buffer[100];
    if (std::strftime(buffer, sizeof(buffer), "%Y/%m/%d %I:%M %p", local_time))
    {
        return std::string(buffer);
    }
    else
    {
        return _("FORMATTING ERROR");
    }
}