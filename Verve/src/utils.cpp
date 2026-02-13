#include "utils.hpp"
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <zlib.h>

using namespace std;

string bytes_to_hex(const string& bytes) {
    stringstream ss;
    ss << setfill('0') << hex;
    for(unsigned char c : bytes) ss << setw(2) << (int)c;
    return ss.str();
}

string sha1_hash(const string& content) {
    unsigned char hash[20];
    SHA1(reinterpret_cast<const unsigned char*>(content.c_str()), content.size(), hash);
    stringstream ss;
    ss << hex << setfill('0');
    for(int i = 0; i < 20; i++) ss << setw(2) << (int)hash[i];
    return ss.str();
}

string zlib_compress(const string& data) {
    z_stream zs;
    zs.zalloc = Z_NULL; zs.zfree = Z_NULL; zs.opaque = Z_NULL;
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) throw runtime_error("Zlib failed");
    
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

map<string,string> deserialize_index(const string& data) {
    map<string, string> index;
    stringstream ss(data);
    string line;
    while(getline(ss, line)) {
        size_t colon = line.find(':');
        if (colon == string::npos) continue;
        index[line.substr(0, colon)] = line.substr(colon + 1);
    }
    return index;
}

string serialize_index(const map<string,string>& index) {
    stringstream ss;
    for (const auto& pair : index) ss << pair.first << ":" << pair.second << "\n";
    return ss.str();
}