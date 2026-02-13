#pragma once 
#include <string>
#include <map>
#include <vector>

// Convert binary bytes to hex string
std::string bytes_to_hex(const std::string& bytes);

// Calculate SHA1 hash
std::string sha1_hash(const std::string& content);

// Compress data using Zlib
std::string zlib_compress(const std::string& data);

// Parse "key:value" text file into a map
std::map<std::string, std::string> deserialize_index(const std::string& data);

// Convert map into "key:value" text
std::string serialize_index(const std::map<std::string, std::string>& index);