#include"Json.h"
#include <sstream>
#include <cctype>

using namespace std;

Json::Json() : type(Type::Null){}

Json::Json(bool b): type(Type::Boolean), boolValue(b){}
Json::Json(int n): type(Type::Number), numberValue(n){}
Json::Json(double d): type(Type::Number), numberValue(d){}
Json::Json(const std::string &s):type(Type::String), stringValue(s){}
Json::Json(const char *c): type(Type::String), stringValue(c){}



Json Json::makeArray()
{
    Json j;
    j.type = Type::Array;
    return j;
}

Json Json::makeObject()
{
    Json j;
    j.type = Type::Object;
    return j;
}

void Json::push_back(const Json &value)
{
    type = Type::Array;
    arrayValue.push_back(value);
}

void Json::set(const std::string &key,const  Json &value)
{
    type = Type::Object;

    for (auto &kv : objectValue)
    {
        if (kv.first == key)
        {
            kv.second = value;
            return;
        }
    }

    objectValue.emplace_back(key, value);
}

bool Json::has(const std::string &key) const
{
    for (auto &kv : objectValue)
        if (kv.first == key)
            return true;

    return false;
}

const Json &Json::operator[](const std::string &key) const
{
    static Json nullValue;

    for (auto &kv : objectValue)
        if (kv.first == key)
            return kv.second;

    return nullValue;
}

const Json &Json::operator[](size_t index) const
{
    static Json nullValue;

    if (index < arrayValue.size())
        return arrayValue[index];

    return nullValue;
}

size_t Json::size() const
{
    if (type == Type::Array)
        return arrayValue.size();

    if (type == Type::Object)
        return objectValue.size();

    return 0;
}

bool Json::asBool(bool def) const
{
    return type == Type::Boolean ? boolValue : def;
}

int Json::asInt(int def) const
{
    return type == Type::Number ? static_cast<int>(numberValue) : def;
}

double Json::asDouble(double def) const
{
    return type == Type::Number ? numberValue : def;
}

std::string Json::asString(const std::string &def) const
{
    return type == Type::String ? stringValue : def;
}

std::string Json::escape(const std::string &s)
{
    ostringstream out;

    for (char c : s)
    {
        switch (c)
        {
        case '"':
            out << "\\\"";
            break;

        case '\\':
            out << "\\\\";
            break;

        case '\n':
            out << "\\n";
            break;

        case '\r':
            out << "\\r";
            break;

        case '\t':
            out << "\\t";
            break;

        default:
            out << c;
        }
    }

    return out.str();
}

std::string Json::dump(int indent) const
{
    std::string pad(static_cast<size_t>(indent) * 2, ' ');
    std::string padInner(static_cast<size_t>(indent + 1) * 2, ' ');

    switch (type)
    {
    case Type::Null:
        return "null";

    case Type::Boolean:
        return boolValue ? "true" : "false";

    case Type::Number:
    {
        if (numberValue == static_cast<long long>(numberValue))
            return std::to_string(static_cast<long long>(numberValue));

        std::ostringstream out;
        out << numberValue;
        return out.str();
    }

    case Type::String:
        return "\"" + escape(stringValue) + "\"";

    case Type::Array:
    {
        if (arrayValue.empty())
            return "[]";

        std::string out = "[\n";

        for (size_t i = 0; i < arrayValue.size(); i++)
        {
            out += padInner + arrayValue[i].dump(indent + 1);
            if (i + 1 < arrayValue.size())
                out += ",";
            out += "\n";
        }

        out += pad + "]";
        return out;
    }

    case Type::Object:
    {
        if (objectValue.empty())
            return "{}";

        std::string out = "{\n";

        for (size_t i = 0; i < objectValue.size(); i++)
        {
            out += padInner + "\"" + escape(objectValue[i].first) + "\": " + objectValue[i].second.dump(indent + 1);
            if (i + 1 < objectValue.size())
                out += ",";
            out += "\n";
        }

        out += pad + "}";
        return out;
    }
    }

    return "null";
}

void Json::skipWhitespace(const std::string &s, size_t &i)
{
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
        i++;
}

std::string Json::parseRawString(const std::string &s, size_t &i)
{
    std::string result;
    i++; 

    while (i < s.size() && s[i] != '"')
    {
        if (s[i] == '\\' && i + 1 < s.size())
        {
            i++;
            switch (s[i])
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
                result += s[i];
            }
        }
        else
        {
            result += s[i];
        }

        i++;
    }

    i++; 
    return result;
}

Json Json::parseValue(const std::string &s, size_t &i)
{
    skipWhitespace(s, i);

    if (i >= s.size())
        return Json();

    char c = s[i];

    if (c == '{')
    {
        Json obj = Json::makeObject();
        i++;

        skipWhitespace(s, i);

        if (i < s.size() && s[i] == '}')
        {
            i++;
            return obj;
        }

        while (i < s.size())
        {
            skipWhitespace(s, i);

            std::string key = parseRawString(s, i);

            skipWhitespace(s, i);

            i++; // skip ':'

            Json value = parseValue(s, i);

            obj.set(key, value);

            skipWhitespace(s, i);

            if (i < s.size() && s[i] == ',')
            {
                i++;
                continue;
            }
            if (i < s.size() && s[i] == '}')
            {
                i++;
                break;
            }
            break;
        }

        return obj;
    }

    if (c == '[')
    {
        Json arr = Json::makeArray();
        i++;
        skipWhitespace(s, i);

        if (i < s.size() && s[i] == ']')
        {
            i++;
            return arr;
        }

        while (i < s.size())
        {
            Json value = parseValue(s, i);
            arr.push_back(value);
            skipWhitespace(s, i);

            if (i < s.size() && s[i] == ',')
            {
                i++;
                continue;
            }
            if (i < s.size() && s[i] == ']')
            {
                i++;
                break;
            }
            break;
        }

        return arr;
    }

    if (c == '"')
        return Json(parseRawString(s, i));

    if (c == 't' && s.compare(i, 4, "true") == 0)
    {
        i += 4;
        return Json(true);
    }
    if (c == 'f' && s.compare(i, 5, "false") == 0)
    {
        i += 5;
        return Json(false);
    }
    if (c == 'n' && s.compare(i, 4, "null") == 0)
    {
        i += 4;
        return Json();
    }

    size_t start = i;
    if (s[i] == '-')
        i++;
    while (i < s.size() && (std::isdigit(static_cast<unsigned char>(s[i])) || s[i] == '.' || s[i] == 'e' || s[i] == 'E' || s[i] == '+' || s[i] == '-'))
        i++;

    return Json(std::stod(s.substr(start, i - start)));
}

Json Json::parse(const std::string &text)
{
    size_t i = 0;
    return parseValue(text, i);
}