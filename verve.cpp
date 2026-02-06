#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream> //create files
#include <iomanip> //format hex values
#include <filesystem> //create folder

#include <openssl/sha.h> //hashing
#include <zlib.h> //compress

namespace fs = std::filesystem;

using namespace std;

// ==========================================
// 1. UTILITIES
// ==========================================

string bytes_to_hex( const string& bytes ){
    stringstream ss;
    ss << setfill('0') << hex;

    for(unsigned char c : bytes){
        ss << setw(2) << (int)c;
    }

    return ss.str();
}

string sha1_hash(const string& content) {
    unsigned char hash[20]; //SHA_DIGEST_LENGTH = 20
   
    SHA1(reinterpret_cast<const unsigned char*>(content.c_str()), content.size(), hash);
    
    stringstream ss;
    ss << hex << setfill('0');
    for(int i = 0; i < 20; i++) {
        ss << setw(2) << (int)hash[i];
    }
    return ss.str();
}

string zlib_compress(const string& data) {
    z_stream zs;                        
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) 
        throw runtime_error("Failed to initialize zlib compression");

    zs.next_in = (Bytef*)data.data();  
    zs.avail_in = data.size();           

    int ret;
    char outbuffer[32768];               
    string outstring;

    do {
        zs.next_out = (Bytef*)outbuffer;
        zs.avail_out = sizeof(outbuffer);
        
        ret = deflate(&zs, Z_FINISH);
        
        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs); 
    return outstring;
}


// ==========================================
// 2. THE REPOSITORY
// ==========================================

class Repository {
public:
    fs::path work_tree; //  ( C:/Users/You/Project)
    fs::path verve_dir; // ( C:/Users/You/Project/.verve)

    Repository(std::string p = ".") {
        work_tree = fs::absolute(p);     
        verve_dir = work_tree / ".verve"; 
    }

    bool init() {
        if (fs::exists(verve_dir)) {
            return false;
        }

        fs::create_directories(verve_dir / "objects");
        fs::create_directories(verve_dir / "refs" / "heads");

        std::ofstream head_file(verve_dir / "HEAD");
        head_file << "ref: refs/heads/master\n";
        head_file.close();

        std::cout << "Initialized empty Verve repository in " << verve_dir << "\n";
        return true;
    }
};


// ==========================================
// 3. MAIN ENTRY POINT
// ==========================================

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: verve <command> [<args>]\n";
        return 1;
    }

    std::string command = argv[1];
    
    Repository repo; 

    try {
        if (command == "init") {
            if (!repo.init()) {
                std::cout << "Repository already exists!\n";
            }
        }
        else {
            std::cout << "Unknown command: " << command << "\n";
        }
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}