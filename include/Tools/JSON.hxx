#pragma once

// Forward declaration of JSONValue
struct JSONValue;

// Type alias for JSON array and object
using JSONArray = std::vector<JSONValue>;
using JSONObject = std::map<std::string, JSONValue>;

// JSONValue definition
struct JSONValue : public std::variant<std::nullptr_t, bool, double, long long, std::string, JSONArray, JSONObject>
{
    using std::variant<std::nullptr_t, bool, double, long long, std::string, JSONArray, JSONObject>::variant;

    // Helper functions to access JSON types
    template <typename T> T get()
    {
        return std::get<T>(*this);
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
        return std::holds_alternative<JSONArray>(*this);
    }

    bool isObject() const
    {
        return std::holds_alternative<JSONObject>(*this);
    }
};

class JSON
{
  public:
    static std::string stringify(const JSONValue &value);
    static JSONValue parse(const std::string &json);
    static JSONValue parse(const std::filesystem::path path);

  private:
    static void stringifyHelper(const JSONValue &value, std::ostringstream &oss, int indentLevel);
    static JSONValue parseValue(const std::string &json, size_t &pos);
    static std::string parseString(const std::string &json, size_t &index);
    static JSONValue parseNumber(const std::string &json, size_t &index);
    static bool parseBool(const std::string &json, size_t &index);
    static std::nullptr_t parseNull(const std::string &json, size_t &index);
    static JSONValue parseObject(const std::string &json, size_t &index);
    static JSONValue parseArray(const std::string &json, size_t &index);
    static void skipWhitespace(const std::string &json, size_t &index);
    static void expect(const std::string &json, size_t &pos, const std::string &expected);
};