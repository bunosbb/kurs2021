#ifndef _PARSER_H_
#define _PARSER_H_

#include "Project.h"

#include "util.h"
class Logger;

enum ObjectType {
    Vector,
    Int,
    String,
    Dict,
    Null
};

class Object {
public:
    bool IsString();
    bool IsInt();
    bool IsNull();
    bool IsVector();
    bool IsDict();

    Object();

    Object(int value);
    Object(std::string value);
    Object(std::vector<Object> value);
    Object(std::unordered_map<std::string, Object*> value);

    void SetType(ObjectType new_type);

    std::string AsString() const;
    int AsInt() const;
    std::vector<Object>& AsVector();
    std::unordered_map<std::string, Object*>& AsDict();

    Object& operator =(std::string other);

    Object& operator =(int other);

    bool ReadString(std::istream& in);
    bool ReadVector(std::istream& in);
    bool ReadDict(std::istream& in);

    bool Read(std::string path);


    void PrintInt(std::ostream& out, std::string tab);
    void PrintString(std::ostream& out, std::string tab);
    void PrintVector(std::ostream& out, std::string tab);
    void PrintDict(std::ostream& out, std::string tab);
    void Print(std::ostream& out, std::string tab);
    void Print(std::string path);

    Object& Get(std::string name);
    Object& Get(int index);
    ~Object();

private:
    ObjectType type;
    std::string str_val;
    std::vector<Object> vector_val;
    std::unordered_map<std::string, Object*> dict_val;
    Logger* log;
    int int_val;
};


#endif