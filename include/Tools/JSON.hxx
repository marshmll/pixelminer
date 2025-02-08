/**
 * @file JSON.hxx
 * @brief Declares the JSON class to handle JSON parsing and generation.
 */

#pragma once

struct JValue;

/**
 * @typedef JArray
 * @brief Type alias for JSON array.
 *
 * Represents a JSON array as a vector of `JValue`.
 */
using JArray = std::vector<JValue>;

/**
 * @typedef JObject
 * @brief Type alias for JSON object.
 *
 * Represents a JSON object as a map of string keys and `JValue` values.
 */
using JObject = std::map<std::string, JValue>;

/**
 * @struct JValue
 * @brief A structure representing a generic JSON value.
 *
 * The `JValue` structure is a wrapper for various types that JSON can represent, including:
 * - null
 * - boolean
 * - integer
 * - real number (double)
 * - string
 * - array
 * - object (map of key-value pairs)
 */
struct JValue : public std::variant<std::nullptr_t, bool, double, long long, std::string, JArray, JObject>
{
    /**
     * @brief Constructor that initializes the value of type T.
     *
     * @param value The value to initialize.
     */
    using std::variant<std::nullptr_t, bool, double, long long, std::string, JArray, JObject>::variant;

    /**
     * @brief Helper function to retrieve the value as a specific type.
     *
     * @tparam T The type to convert the value to.
     * @return T The value converted to the specified type.
     * @throws std::runtime_error If the current value cannot be converted to the desired type.
     */
    template <typename T> T getAs()
    {
        if (std::holds_alternative<T>(*this))
            return std::get<T>(*this);

        throw std::runtime_error("[ JSONValue ] -> Tried to convert value to an invalid type.");
    }

    /**
     * @brief Checks if the current value is `null`.
     *
     * @return True if the value is `null`, false otherwise.
     */
    bool isNull() const
    {
        return std::holds_alternative<std::nullptr_t>(*this);
    }

    /**
     * @brief Checks if the current value is a `bool`.
     *
     * @return True if the value is a boolean, false otherwise.
     */
    bool isBool() const
    {
        return std::holds_alternative<bool>(*this);
    }

    /**
     * @brief Checks if the current value is a real number (double).
     *
     * @return True if the value is a real number, false otherwise.
     */
    bool isRealNumber() const
    {
        return std::holds_alternative<double>(*this);
    }

    /**
     * @brief Checks if the current value is an integer (long long).
     *
     * @return True if the value is an integer, false otherwise.
     */
    bool isIntegerNumber() const
    {
        return std::holds_alternative<long long>(*this);
    }

    /**
     * @brief Checks if the current value is a `string`.
     *
     * @return True if the value is a string, false otherwise.
     */
    bool isString() const
    {
        return std::holds_alternative<std::string>(*this);
    }

    /**
     * @brief Checks if the current value is an array.
     *
     * @return True if the value is an array, false otherwise.
     */
    bool isArray() const
    {
        return std::holds_alternative<JArray>(*this);
    }

    /**
     * @brief Checks if the current value is an object.
     *
     * @return True if the value is an object, false otherwise.
     */
    bool isObject() const
    {
        return std::holds_alternative<JObject>(*this);
    }
};

/**
 * @class JSON
 * @brief JSON utility class for parsing and stringifying JSON data.
 */
class JSON
{
  public:
    /**
     * @brief Converts a `JValue` to a JSON string representation.
     *
     * @param value The `JValue` to convert.
     * @return A JSON string representation of the value.
     */
    static std::string stringify(const JValue &value);

    /**
     * @brief Parses a JSON string into a `JValue`.
     *
     * @param json The JSON string to parse.
     * @return The parsed `JValue`.
     * @throws std::runtime_error If the JSON string is malformed.
     */
    static JValue parse(const std::string &json);

    /**
     * @brief Parses a JSON file into a `JValue`.
     *
     * @param path The path to the JSON file.
     * @return The parsed `JValue`.
     * @throws std::runtime_error If the file cannot be opened or the JSON is malformed.
     */
    static JValue parse(const std::filesystem::path path);

  private:
    /**
     * @brief Helper function to assist with stringifying a `JValue` recursively.
     *
     * @param value The `JValue` to stringify.
     * @param oss The output stream to write the string representation to.
     * @param indentLevel The current indentation level for formatting.
     */
    static void stringifyHelper(const JValue &value, std::ostringstream &oss, int indentLevel);

    /**
     * @brief Helper function to parse a JSON value from a string.
     *
     * @param json The JSON string to parse.
     * @param pos The current position in the string.
     * @return The parsed `JValue`.
     * @throws std::runtime_error If the JSON value is invalid.
     */
    static JValue parseValue(const std::string &json, size_t &pos);

    /**
     * @brief Helper function to parse a JSON string value.
     *
     * @param json The JSON string to parse.
     * @param index The current position in the string.
     * @return The parsed string.
     * @throws std::runtime_error If the string is invalid.
     */
    static std::string parseString(const std::string &json, size_t &index);

    /**
     * @brief Helper function to parse a JSON number value.
     *
     * @param json The JSON string to parse.
     * @param index The current position in the string.
     * @return The parsed number as a `JValue`.
     * @throws std::runtime_error If the number is invalid.
     */
    static JValue parseNumber(const std::string &json, size_t &index);

    /**
     * @brief Helper function to parse a JSON boolean value.
     *
     * @param json The JSON string to parse.
     * @param index The current position in the string.
     * @return The parsed boolean value.
     * @throws std::runtime_error If the boolean is invalid.
     */
    static bool parseBool(const std::string &json, size_t &index);

    /**
     * @brief Helper function to parse a JSON null value.
     *
     * @param json The JSON string to parse.
     * @param index The current position in the string.
     * @return The parsed null value.
     * @throws std::runtime_error If the null value is invalid.
     */
    static std::nullptr_t parseNull(const std::string &json, size_t &index);

    /**
     * @brief Helper function to parse a JSON object.
     *
     * @param json The JSON string to parse.
     * @param pos The current position in the string.
     * @return The parsed `JObject`.
     * @throws std::runtime_error If the object is invalid.
     */
    static JValue parseObject(const std::string &json, size_t &index);

    /**
     * @brief Helper function to parse a JSON array.
     *
     * @param json The JSON string to parse.
     * @param pos The current position in the string.
     * @return The parsed `JArray`.
     * @throws std::runtime_error If the array is invalid.
     */
    static JValue parseArray(const std::string &json, size_t &index);

    /**
     * @brief Helper function to skip over whitespace in a JSON string.
     *
     * @param json The JSON string to process.
     * @param pos The current position in the string.
     */
    static void skipWhitespace(const std::string &json, size_t &index);

    /**
     * @brief Helper function to check that the current position in the string matches an expected substring.
     *
     * @param json The JSON string to process.
     * @param pos The current position in the string.
     * @param expected The expected substring.
     * @throws std::runtime_error If the substring does not match.
     */
    static void expect(const std::string &json, size_t &pos, const std::string &expected);
};
