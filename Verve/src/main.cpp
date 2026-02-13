#include <iostream>
#include <string>
#include "repository.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: verve <command> [<args>]\n";
        return 1;
    }

    string command = argv[1];
    Repository repo;

    try {
        if (command == "init") {
            if (!repo.init()) cout << "Repository already exists!\n";
        }
        else if (command == "add") {
            if (argc < 3) {
                cout << "Usage: verve add <filename>\n";
                return 1;
            }
            repo.add_file(argv[2]);
        }
        else {
            cout << "Unknown command: " << command << "\n";
        }
    } 
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}