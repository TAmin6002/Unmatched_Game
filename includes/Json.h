#pragma once

#include <string>
#include <vector>
#include <utility>
#include"Enums.h"

class Json
{
    private:
        Type type;
        bool boolValue = false;
        double numberValue = 0.0;
        std::string stringValue;
        std::vector<Json> arrayValue;
        std::vector<std::pair<std::string, Json>> objectValue;
    
        static std::string escape(const std::string &);
    
        static void skipWhitespace(const std::string &, size_t &);
        static std::string parseRawString(const std::string &, size_t &);
        static Json parseValue(const std::string &, size_t &);

public:

    Json();
    Json(bool);
    Json(int);
    Json(double);
    Json(const std::string &);
    Json(const char *);

    static Json makeArray();
    static Json makeObject();


    void push_back(const Json &);

    void set(const std::string &,const Json &);
    bool has(const std::string &) const;

    const Json &operator[](const std::string &) const;
    const Json &operator[](size_t ) const;

    size_t size() const;

    bool asBool(bool  = false) const;
    int asInt(int  = 0) const;
    double asDouble(double  = 0.0) const;
    std::string asString(const std::string & = "") const;

    Type getType() const { return type; }

    std::string dump(int  = 0) const;
    static Json parse(const std::string &);

};