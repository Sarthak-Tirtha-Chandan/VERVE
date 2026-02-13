#pragma once
#include <string>
#include <map>
#include <filesystem>
#include "verve_objects.hpp"

namespace fs = std::filesystem;

class Repository {
public:
    fs::path work_tree;
    fs::path verve_dir;
    fs::path objects_dir;
    fs::path index_file;

    Repository(std::string p = ".");

    bool init();
    
    // Index operations
    std::map<std::string, std::string> load_index();
    void save_index(const std::map<std::string, std::string>& index);
    
    // Store object to disk
    std::string store_object(const VerveObject& obj);
    
    // Add command
    void add_file(std::string path_str);
};