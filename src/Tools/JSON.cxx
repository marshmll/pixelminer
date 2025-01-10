#include "Tools/JSON.hxx"
#include "stdafx.hxx"

std::string JSON::stringify(const JSONValue &value)
{
    std::ostringstream oss;
    stringifyHelper(value, oss, 1);
    return oss.str();
}

void JSON::stringifyHelper(const JSONValue &value, std::ostringstream &oss, int indentLevel)
{
    const std::string indent(indentLevel * 4, ' ');
    const std::string newLine = "\n";

    if (value.isNull())
    {
        oss << "null";
    }
    else if (value.isBool())
    {
        oss << (std::get<bool>(value) ? "true" : "false");
    }
    else if (value.isRealNumber())
    {
        oss << std::get<double>(value);
    }
    else if (value.isIntegerNumber())
    {
        oss << std::get<long long>(value);
    }
    else if (value.isString())
    {
        oss << '"' << std::get<std::string>(value) << '"';
    }
    else if (value.isArray())
    {
        oss << "[" << newLine;
        const auto &array = std::get<JSONArray>(value);
        for (size_t i = 0; i < array.size(); ++i)
        {
            if (i > 0)
                oss << "," << newLine;
            oss << indent;
            stringifyHelper(array[i], oss, indentLevel + 1);
        }
        oss << newLine << std::string((indentLevel - 1) * 4, ' ') << "]";
    }
    else if (value.isObject())
    {
        oss << "{" << newLine;
        const auto &object = std::get<JSONObject>(value);
        bool first = true;
        for (const auto &[key, val] : object)
        {
            if (!first)
                oss << "," << newLine;
            first = false;
            oss << indent << '"' << key << "\": ";
            stringifyHelper(val, oss, indentLevel + 1);
        }
        oss << newLine << std::string((indentLevel - 1) * 4, ' ') << "}";
    }
}

JSONValue JSON::parse(const std::string &json)
{
    size_t index = 0;
    skipWhitespace(json, index);
    return parseValue(json, index);
}

JSONValue JSON::parse(const std::filesystem::path path)
{
    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("[ JSON ] -> Could not open file: " + path.string() + "\n");

    std::ostringstream oss;
    oss << file.rdbuf();
    file.close();

    return parse(oss.str());
}

JSONValue JSON::parseValue(const std::string &json, size_t &pos)
{
    skipWhitespace(json, pos);

    if (pos >= json.size())
    {
        throw std::runtime_error("[ JSON ] -> Unexpected end of input");
    }

    if (json[pos] == 'n')
    {
        expect(json, pos, "null");
        return JSONValue(nullptr);
    }
    else if (json[pos] == 't')
    {
        expect(json, pos, "true");
        return JSONValue(true);
    }
    else if (json[pos] == 'f')
    {
        expect(json, pos, "false");
        return JSONValue(false);
    }
    else if (json[pos] == '"')
    {
        return JSONValue(parseString(json, pos));
    }
    else if (json[pos] == '[')
    {
        return parseArray(json, pos);
    }
    else if (json[pos] == '{')
    {
        return parseObject(json, pos);
    }
    else if (std::isdigit(json[pos]) || json[pos] == '-')
    {
        return parseNumber(json, pos);
    }

    throw std::runtime_error("[ JSON ] -> Invalid JSON value at position " + std::to_string(pos));
}

std::string JSON::parseString(const std::string &json, size_t &pos)
{
    if (json[pos] != '"')
        throw std::runtime_error("[ JSON ] -> Expected '\"' at position " + std::to_string(pos));
    ++pos;

    std::string result;
    while (pos < json.size() && json[pos] != '"')
    {
        if (json[pos] == '\\')
        {
            ++pos;
            if (pos >= json.size())
                throw std::runtime_error("[ JSON ] -> Unexpected end of input in string");
            switch (json[pos])
            {
            case '"':
                result += '"';
                break;
            case '\\':
                result += '\\';
                break;
            case '/':
                result += '/';
                break;
            case 'b':
                result += '\b';
                break;
            case 'f':
                result += '\f';
                break;
            case 'n':
                result += '\n';
                break;
            case 'r':
                result += '\r';
                break;
            case 't':
                result += '\t';
                break;
            default:
                throw std::runtime_error("[ JSON ] -> Invalid escape character in string");
            }
        }
        else
        {
            result += json[pos];
        }
        ++pos;
    }

    if (pos >= json.size() || json[pos] != '"')
        throw std::runtime_error("[ JSON ] -> Unterminated string");
    ++pos;

    return result;
}

JSONValue JSON::parseNumber(const std::string &json, size_t &pos)
{
    size_t start = pos;

    // Handle optional '-' for negative numbers
    if (json[pos] == '-')
    {
        ++pos;
    }

    // Parse digits before the decimal point or exponent
    bool isInteger = true;
    while (pos < json.size() && std::isdigit(json[pos]))
    {
        ++pos;
    }

    // Check for fractional part
    if (pos < json.size() && json[pos] == '.')
    {
        isInteger = false;
        ++pos;
        while (pos < json.size() && std::isdigit(json[pos]))
        {
            ++pos;
        }
    }

    // Check for exponent
    if (pos < json.size() && (json[pos] == 'e' || json[pos] == 'E'))
    {
        isInteger = false;
        ++pos;
        if (pos < json.size() && (json[pos] == '+' || json[pos] == '-'))
        {
            ++pos;
        }
        while (pos < json.size() && std::isdigit(json[pos]))
        {
            ++pos;
        }
    }

    // Parse the number
    std::string numberStr = json.substr(start, pos - start);
    if (isInteger)
    {
        try
        {
            long long value = std::stoll(numberStr); // Parse as long long
            return JSONValue(value);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error("[ JSON ] -> Integer value out of range: " + numberStr);
        }
    }
    else
    {
        try
        {
            double value = std::stod(numberStr); // Parse as double
            return JSONValue(value);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error("[ JSON ] -> Floating-point value out of range: " + numberStr);
        }
    }
}

bool JSON::parseBool(const std::string &json, size_t &index)
{
    if (json.substr(index, 4) == "true")
    {
        index += 4;
        return true;
    }
    else if (json.substr(index, 5) == "false")
    {
        index += 5;
        return false;
    }
    throw std::runtime_error("[ JSON ] -> Invalid boolean value");
}

std::nullptr_t JSON::parseNull(const std::string &json, size_t &index)
{
    if (json.substr(index, 4) == "null")
    {
        index += 4;
        return nullptr;
    }
    throw std::runtime_error("[ JSON ] -> Invalid null value");
}

JSONValue JSON::parseObject(const std::string &json, size_t &pos)
{
    skipWhitespace(json, pos);
    if (json[pos] != '{')
        throw std::runtime_error("[ JSON ] -> Expected '{' at position " + std::to_string(pos));
    ++pos;

    JSONObject object;
    skipWhitespace(json, pos);
    if (json[pos] == '}')
    {
        ++pos;
        return JSONValue(object);
    }

    while (true)
    {
        skipWhitespace(json, pos);
        if (json[pos] != '"')
            throw std::runtime_error("[ JSON ] -> Expected '\"' at position " + std::to_string(pos));
        std::string key = parseString(json, pos);

        skipWhitespace(json, pos);
        if (json[pos] != ':')
            throw std::runtime_error("[ JSON ] -> Expected ':' at position " + std::to_string(pos));
        ++pos;

        object[key] = parseValue(json, pos);
        skipWhitespace(json, pos);

        if (json[pos] == '}')
        {
            ++pos;
            return JSONValue(object);
        }
        else if (json[pos] == ',')
        {
            ++pos;
        }
        else
        {
            throw std::runtime_error("[ JSON ] -> Expected ',' or '}' at position " + std::to_string(pos));
        }
    }
}

JSONValue JSON::parseArray(const std::string &json, size_t &pos)
{
    skipWhitespace(json, pos);
    if (json[pos] != '[')
        throw std::runtime_error("[ JSON ] -> Expected '[' at position " + std::to_string(pos));
    ++pos;

    JSONArray array;
    skipWhitespace(json, pos);
    if (json[pos] == ']')
    {
        ++pos;
        return JSONValue(array);
    }

    while (true)
    {
        array.push_back(parseValue(json, pos));
        skipWhitespace(json, pos);

        if (json[pos] == ']')
        {
            ++pos;
            return JSONValue(array);
        }
        else if (json[pos] == ',')
        {
            ++pos;
        }
        else
        {
            throw std::runtime_error("[ JSON ] -> Expected ',' or ']' at position " + std::to_string(pos));
        }
    }
}

void JSON::skipWhitespace(const std::string &json, size_t &pos)
{
    while (pos < json.size() && isspace(json[pos]))
        ++pos;
}

void JSON::expect(const std::string &json, size_t &pos, const std::string &expected)
{
    size_t len = expected.length();
    if (json.substr(pos, len) != expected)
    {
        throw std::runtime_error("[ JSON ] -> Expected '" + expected + "' at position " + std::to_string(pos));
    }
    pos += len;
}
