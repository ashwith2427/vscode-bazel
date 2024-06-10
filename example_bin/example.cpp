#include <fstream>
#include <iostream>
#include <parser/parser.hpp>
#include "tools/cpp/runfiles/runfiles.h"

int main(int argc, char** argv) {
    using bazel::tools::cpp::runfiles::Runfiles;
    std::string error;
    std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv[0], &error));
    std::string path = runfiles->Rlocation("_main/example_bin/data/Roboto-Black.ttf");

    std::ifstream* file=new std::ifstream(path,std::ios::in | std::ios::binary);
    if (!file->is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return 1;
    }
    FontReader* reader=new FontReader(*file);
    std::vector<point> points=reader->ParseFont();
    for(auto& i:points){
        std::cout<<i;
    }
    return 0;
}
