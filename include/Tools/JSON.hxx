#pragma once

// Forward declaration of JSONValue
struct JValue;

// Type alias for JSON array and object
using JArray = std::vector<JValue>;
using JObject = std::map<std::string, JValue>;

// JSONValue definition
struct JValue : public std::variant<std::nullptr_t, bool, double, long long, std::string, JArray, JObject>
{
    using std::variant<std::nullptr_t, bool, double, long long, std::string, JArray, JObject>::variant;

    // Helper functions to access JSON types
    template <typename T> T getAs()
    {
        if (std::holds_alternative<T>(*this))
            return std::get<T>(*this);

        throw std::runtime_error("[ JSONValue ] -> Variant type mismatch.");
    }

    bool isNull() const
    {
        return std::holds_alternative<std::nullptr_t>(*this);
    }

    bool isBool() const
    {
        return std::holds_alternative<bool>(*this);
    }

    bool isRealNumber() const
    {
        return std::holds_alternative<double>(*this);
    }

    bool isIntegerNumber() const
    {
        return std::holds_alternative<long long>(*this);
    }

    bool isString() const
    {
        return std::holds_alternative<std::string>(*this);
    }

    bool isArray() const
    {
        return std::holds_alternative<JArray>(*this);
    }

    bool isObject() const
    {
        return std::holds_alternative<JObject>(*this);
    }
};

class JSON
{
  public:
    static std::string stringify(const JValue &value);
    static JValue parse(const std::string &json);
    static JValue parse(const std::filesystem::path path);

  private:
    static void stringifyHelper(const JValue &value, std::ostringstream &oss, int indentLevel);
    static JValue parseValue(const std::string &json, size_t &pos);
    static std::string parseString(const std::string &json, size_t &index);
    static JValue parseNumber(const std::string &json, size_t &index);
    static bool parseBool(const std::string &json, size_t &index);
    static std::nullptr_t parseNull(const std::string &json, size_t &index);
    static JValue parseObject(const std::string &json, size_t &index);
    static JValue parseArray(const std::string &json, size_t &index);
    static void skipWhitespace(const std::string &json, size_t &index);
    static void expect(const std::string &json, size_t &pos, const std::string &expected);
};