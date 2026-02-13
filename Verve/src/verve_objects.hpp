#pragma once
#include <string>

class VerveObject {
public:
    std::string type;
    std::string content;

    VerveObject(std::string t, std::string c);
    
    // Calculates the SHA1 hash of (Header + Content)
    std::string hash() const;
    
    // Compresses (Header + Content)
    std::string serialize() const;
};

class Blob : public VerveObject {
public:
    Blob(std::string content);
};