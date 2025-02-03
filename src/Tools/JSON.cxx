#include "Tools/JSON.hxx"
#include "stdafx.hxx"
#include <iomanip>
#include <sstream>

// Helper function to calculate line and column numbers
std::pair<size_t, size_t> getLineAndColumn(const std::string &json, size_t pos)
{
    size_t line = 1;
    size_t column = 1;

    for (size_t i = 0; i < pos && i < json.size(); ++i)
    {
        if (json[i] == '\n')
        {
            ++line;
            column = 1;
        }
        else
        {
            ++column;
        }
    }

    return {line, column};
}

// Helper function to generate an error message with context and an arrow
std::string generateErrorMessage(const std::string &json, size_t pos, const std::string &message)
{
    auto [line, column] = getLineAndColumn(json, pos);

    // Find the start and end of the line containing the error
    size_t lineStart = json.rfind('\n', pos);
    if (lineStart == std::string::npos)
    {
        lineStart = 0;
    }
    else
    {
        lineStart++; // Move past the newline character
    }

    size_t lineEnd = json.find('\n', pos);
    if (lineEnd == std::string::npos)
    {
        lineEnd = json.size();
    }

    // Extract the line content
    std::string lineContent = json.substr(lineStart, lineEnd - lineStart);

    // Create the arrow pointing to the error position
    std::string arrow(column - 1, ' ');
    arrow += "^";

    // Build the error message
    std::ostringstream oss;
    oss << "[ JSON ] -> Error at line " << line << ", column " << column << ":\n"
        << "    " << lineContent << "\n"
        << "    " << arrow << "\n"
        << "    " << message << "\n\n";

    return oss.str();
}

std::string JSON::stringify(const JValue &value)
{
    std::ostringstream oss;
    stringifyHelper(value, oss, 1);
    return oss.str();
}

void JSON::stringifyHelper(const JValue &value, std::ostringstream &oss, int indentLevel)
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
        const auto &array = std::get<JArray>(value);
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
        const auto &object = std::get<JObject>(value);
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

JValue JSON::parse(const std::string &json)
{
    size_t index = 0;
    skipWhitespace(json, index);
    return parseValue(json, index);
}

JValue JSON::parse(const std::filesystem::path path)
{
    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("[ JSON ] -> Could not open file: " + path.string() + "\n");

    std::ostringstream oss;
    oss << file.rdbuf();
    file.close();

    return parse(oss.str());
}

JValue JSON::parseValue(const std::string &json, size_t &pos)
{
    skipWhitespace(json, pos);

    if (pos >= json.size())
    {
        throw std::runtime_error(generateErrorMessage(json, pos, "Unexpected end of input"));
    }

    if (json[pos] == 'n')
    {
        expect(json, pos, "null");
        return JValue(nullptr);
    }
    else if (json[pos] == 't')
    {
        expect(json, pos, "true");
        return JValue(true);
    }
    else if (json[pos] == 'f')
    {
        expect(json, pos, "false");
        return JValue(false);
    }
    else if (json[pos] == '"')
    {
        return JValue(parseString(json, pos));
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

    throw std::runtime_error(generateErrorMessage(json, pos, "Invalid JSON value"));
}

std::string JSON::parseString(const std::string &json, size_t &pos)
{
    if (json[pos] != '"')
        throw std::runtime_error(generateErrorMessage(json, pos, "Expected '\"'"));
    ++pos;

    std::string result;
    while (pos < json.size() && json[pos] != '"')
    {
        if (json[pos] == '\\')
        {
            ++pos;
            if (pos >= json.size())
                throw std::runtime_error(generateErrorMessage(json, pos, "Unexpected end of input in string"));
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
                throw std::runtime_error(generateErrorMessage(json, pos, "Invalid escape character in string"));
            }
        }
        else
        {
            result += json[pos];
        }
        ++pos;
    }

    if (pos >= json.size() || json[pos] != '"')
        throw std::runtime_error(generateErrorMessage(json, pos, "Unterminated string"));
    ++pos;

    return result;
}

JValue JSON::parseNumber(const std::string &json, size_t &pos)
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
            return JValue(value);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error(generateErrorMessage(json, pos, "Integer value out of range: " + numberStr));
        }
    }
    else
    {
        try
        {
            double value = std::stod(numberStr); // Parse as double
            return JValue(value);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error(
                generateErrorMessage(json, pos, "Floating-point value out of range: " + numberStr));
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
    throw std::runtime_error(generateErrorMessage(json, index, "Invalid boolean value"));
}

std::nullptr_t JSON::parseNull(const std::string &json, size_t &index)
{
    if (json.substr(index, 4) == "null")
    {
        index += 4;
        return nullptr;
    }
    throw std::runtime_error(generateErrorMessage(json, index, "Invalid null value"));
}

JValue JSON::parseObject(const std::string &json, size_t &pos)
{
    skipWhitespace(json, pos);
    if (json[pos] != '{')
        throw std::runtime_error(generateErrorMessage(json, pos, "Expected '{'"));
    ++pos;

    JObject object;
    skipWhitespace(json, pos);
    if (json[pos] == '}')
    {
        ++pos;
        return JValue(object);
    }

    while (true)
    {
        skipWhitespace(json, pos);
        if (json[pos] != '"')
            throw std::runtime_error(generateErrorMessage(json, pos, "Expected '\"'"));
        std::string key = parseString(json, pos);

        skipWhitespace(json, pos);
        if (json[pos] != ':')
            throw std::runtime_error(generateErrorMessage(json, pos, "Expected ':'"));
        ++pos;

        object[key] = parseValue(json, pos);
        skipWhitespace(json, pos);

        if (json[pos] == '}')
        {
            ++pos;
            return JValue(object);
        }
        else if (json[pos] == ',')
        {
            ++pos;
        }
        else
        {
            throw std::runtime_error(generateErrorMessage(json, pos, "Expected ',' or '}'"));
        }
    }
}

JValue JSON::parseArray(const std::string &json, size_t &pos)
{
    skipWhitespace(json, pos);
    if (json[pos] != '[')
        throw std::runtime_error(generateErrorMessage(json, pos, "Expected '['"));
    ++pos;

    JArray array;
    skipWhitespace(json, pos);
    if (json[pos] == ']')
    {
        ++pos;
        return JValue(array);
    }

    while (true)
    {
        array.push_back(parseValue(json, pos));
        skipWhitespace(json, pos);

        if (json[pos] == ']')
        {
            ++pos;
            return JValue(array);
        }
        else if (json[pos] == ',')
        {
            ++pos;
        }
        else
        {
            throw std::runtime_error(generateErrorMessage(json, pos, "Expected ',' or ']'"));
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
        throw std::runtime_error(generateErrorMessage(json, pos, "Expected '" + expected + "'"));
    }
    pos += len;
}