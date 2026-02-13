#include "repository.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Repository::Repository(string p) {
    work_tree = fs::absolute(p);
    verve_dir = work_tree / ".verve";
    objects_dir = verve_dir / "objects";
    index_file = verve_dir / "index";
}

bool Repository::init() {
    if (fs::exists(verve_dir)) return false;

    fs::create_directories(objects_dir);
    fs::create_directories(verve_dir / "refs" / "heads");

    ofstream head_file(verve_dir / "HEAD");
    head_file << "ref: refs/heads/master\n";
    head_file.close();

    cout << "Initialized empty Verve repository in " << verve_dir << "\n";
    return true;
}

map<string, string> Repository::load_index() {
    if (!fs::exists(index_file)) return {};
    ifstream f(index_file);
    stringstream buffer;
    buffer << f.rdbuf();
    return deserialize_index(buffer.str());
}

void Repository::save_index(const map<string, string>& index) {
    ofstream f(index_file);
    f << serialize_index(index);
}

string Repository::store_object(const VerveObject& obj) {
    string obj_hash = obj.hash();
    fs::path dir = objects_dir / obj_hash.substr(0, 2);
    fs::path file = dir / obj_hash.substr(2);

    if (!fs::exists(file)) {
        fs::create_directories(dir);
        ofstream f(file, ios::binary);
        f << obj.serialize();
    }
    return obj_hash;
}

void Repository::add_file(string path_str) {
    fs::path target = work_tree / path_str;
    if (!fs::exists(target)) throw runtime_error("File not found: " + path_str);

    ifstream f(target, ios::binary);
    stringstream buffer;
    buffer << f.rdbuf();

    Blob blob(buffer.str());
    string h = store_object(blob);

    auto index = load_index();
    index[fs::relative(target, work_tree).string()] = h;
    save_index(index);

    cout << "Added " << path_str << " (Hash: " << h << ")\n";
}