#include "Parser.h"

bool Get_char(std::istream& in, char& ch) {

    while (in >> ch) {
        if (ch != ' ') {
            return true;
        }
    }
    return false;
}

Object::Object() : type(Null), log(new Logger) {}

Object::Object(int value) : type(Int), int_val(value) {}
Object::Object(std::string value) : type(String), str_val(value) {}
Object::Object(std::vector<Object> value) : type(Vector) {
    std::swap(vector_val, value);
}
Object::Object(std::unordered_map<std::string, Object*> value) : type(Dict) {
    std::swap(dict_val, value);
}

void Object::SetType(ObjectType new_type) {
    type = new_type;
    str_val = "";
    dict_val.clear();
    vector_val.clear();
}

bool Object::IsString() {
    return type == String;
}
bool Object::IsInt() {
    return type == Int;
}
bool Object::IsNull() {
    return type == Null;
}
bool Object::IsVector() {
    return type == Vector;
}
bool Object::IsDict() {
    return type == Dict;
}

std::string Object::AsString() const  {
    return str_val;
}
int Object::AsInt() const  {
    return int_val;
}
std::vector<Object>& Object::AsVector() {
    return vector_val;
}
std::unordered_map<std::string, Object*>& Object::AsDict() {
    return dict_val;
}

Object& Object::operator =(std::string other) {
    type = String;
    str_val = other;
    return *this;
}


Object& Object::operator =(int other) {
    type = Int;
    int_val = other;
    return *this;
}

bool Object::ReadString(std::istream& in) {
    type = String;
    char in_ch;
    while (in >> in_ch) {
        if (in_ch == '"')
            return true;
        str_val += in_ch;
    }
    return false;
}
bool Object::ReadVector(std::istream& in) {
    type = Vector;
    char in_ch;
    Get_char(in, in_ch);
    while (in_ch != ']') {
        Object item;
        switch (in_ch) {
        case '"':
            item.ReadString(in);
            break;
        case '[':
            item.ReadVector(in);
            break;
        case '{':
            item.ReadDict(in);
            break;
        case 'n':
            Get_char(in, in_ch);
            if (in_ch != 'u')
                return false;
            Get_char(in, in_ch);
            if (in_ch != 'l')
                return false;
            Get_char(in, in_ch);
            if (in_ch != 'l')
                return false;
            item.type = Null;
            break;
        default:
            std::string int_str;
            while ('0' <= in_ch && in_ch <= '9') {
                int_str += in_ch;
                in_ch = in.peek();
                if ('0' > in_ch || in_ch > '9') {
                    break;
                }
                Get_char(in, in_ch);
            }
            item.type = Int;
            item.int_val = std::stoi(int_str);
            break;
        }
        vector_val.push_back(item);
        Get_char(in, in_ch);
        if (in_ch == ']')
            return true;
        if (in_ch != ',') {
            std::cerr << "cant read vector\n";
            (*log)("Parser", "ReadVector", "ERROR, Can't read vector");
            return false;
        }
        Get_char(in, in_ch);
    }
    return true;
}

bool Object::ReadDict(std::istream& in) {
    type = Dict;
    char in_ch;
    Get_char(in, in_ch);
    while (in_ch != '}') {
        if (in_ch != '"') {
            (*log)("Parser", "ReadDict", "ERROR, can't read dict. There is no key");
            return false;
        }
        Object name;
        name.ReadString(in);
        Get_char(in, in_ch);
        if (in_ch != ':') {
            (*log)("Parser", "ReadDict", "ERROR, can't read dict. Wrong sytax with key " + name.AsString());
            return false;
        }
        Get_char(in, in_ch);
        Object* item = new Object;
        switch (in_ch) {
        case '"':
            item->ReadString(in);
            break;
        case '[':
            item->ReadVector(in);
            break;
        case '{':
            item->ReadDict(in);
            break;
        case 'n':
            Get_char(in, in_ch);
            if (in_ch != 'u')
                return false;
            Get_char(in, in_ch);
            if (in_ch != 'l')
                return false;
            Get_char(in, in_ch);
            if (in_ch != 'l')
                return false;
            item->type = Null;
            break;
        default:
            std::string int_str;
            while ('0' <= in_ch && in_ch <= '9') {
                int_str += in_ch;
                in_ch = in.peek();
                if ('0' > in_ch || in_ch > '9') {
                    break;
                }
                Get_char(in, in_ch);
            }
            item->type = Int;
            item->int_val = std::stoi(int_str);
            break;
        }
        dict_val[name.AsString()] = item;
        Get_char(in, in_ch);
        if (in_ch == '}')
            return true;
        if (in_ch != ',') {
            (*log)("Parser", "ReadDict", "ERROR, can't read dict.");
            return false;
        }
        Get_char(in, in_ch);
    }
    return true;
}

bool Object::Read(std::string path) {
    std::ifstream in(path);
    char in_ch;
    if (!Get_char(in, in_ch)) {
        return true;
    }
    switch (in_ch) {
    case '"':
        return ReadString(in);
        break;
    case '[':
        return ReadVector(in);
        break;
    case '{':
        return ReadDict(in);
        break;
    case 'n':
        Get_char(in, in_ch);
        if (in_ch != 'u')
            return false;
        Get_char(in, in_ch);
        if (in_ch != 'l')
            return false;
        Get_char(in, in_ch);
        if (in_ch != 'l')
            return false;
        type = Null;
        return true;
    default:
        std::string int_str;
        while ('0' <= in_ch && in_ch <= '9') {
            int_str += in_ch;
            in_ch = in.peek();
            if ('0' > in_ch || in_ch > '9') {
                break;
            }
            Get_char(in, in_ch);
        }
        type = Int;
        int_val = std::stoi(int_str);
        return true;
    }
    return false;
}

void Object::PrintInt(std::ostream& out, std::string given_tab) {
    out << int_val;
}
void Object::PrintString(std::ostream& out, std::string given_tab) {
    out << '"' << str_val << '"';
}
void Object::PrintVector(std::ostream& out, std::string given_tab) {
    std::string tab = given_tab + "  ";
    out << "[\n";
    for (auto i = 0; i < vector_val.size(); ++i) {
        out << tab;
        vector_val[i].Print(out, tab);
        if (i + 1 < vector_val.size()) {
            out << ',';
        }
        out << '\n';
    }
    out << given_tab << "]";
}
void Object::PrintDict(std::ostream& out, std::string given_tab) {
    std::string tab = given_tab + "  ";
    out << "{\n";
    for (auto it = dict_val.begin(); it != dict_val.end(); ++it) {
        if (it != dict_val.begin()) {
            out << ",\n";
        }
        out << tab;
        out << '"' << it->first << '"' <<  ": ";
        it->second->Print(out, tab);
    }
    out << '\n' << given_tab  << "}";
}

void Object::Print(std::ostream& out, std::string tab) {
    switch (type) {
    case Int:
        PrintInt(out, tab);
        break;
    case String:
        PrintString(out, tab);
        break;
    case Vector:
        PrintVector(out, tab);
        break;
    case Dict:
        PrintDict(out, tab);
        break;
    case Null:
        out << "null";
        break;
    }
}
void Object::Print(std::string path) {
    std::ofstream out(path);
    Print(out, "");
}

Object& Object::Get(std::string name) {
    if (type != Dict) {
        (*log)("Parser", "Read", "Not dict Object. Key: " + name);
        //throw std::exception();
    }
    if (dict_val.find(name) == dict_val.end()) {
        (*log)("Parser", "Read", "No such key in the dict. Key: " + name);
        //throw std::exception();
    }
    return *dict_val[name];
}

Object& Object::Get(int index) {
    if (type != Vector) {
        (*log)("Parser", "Read", "Not vector Object. Index: " + std::to_string(index));
        //throw std::exception();
    }
    if (index >= vector_val.size()) {
        (*log)("Parser", "Read", "No such index in vector. Index: " + std::to_string(index));
        //throw std::exception();
    }
    return vector_val[index];
}

Object::~Object() {
}