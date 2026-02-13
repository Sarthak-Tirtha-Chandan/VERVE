#include "verve_objects.hpp"
#include "utils.hpp" 

using namespace std;

VerveObject::VerveObject(string t, string c) : type(t), content(c) {}

string VerveObject::hash() const {
    string header = type + " " + to_string(content.size()) + '\0';
    return sha1_hash(header + content);
}

string VerveObject::serialize() const {
    string header = type + " " + to_string(content.size()) + '\0';
    return zlib_compress(header + content);
}

Blob::Blob(string content) : VerveObject("blob", content) {}