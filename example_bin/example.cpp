#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <ios>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "tools/cpp/runfiles/runfiles.h"



int main(int argc, char** argv) {
    using bazel::tools::cpp::runfiles::Runfiles;
    std::string error;
    std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv[0], &error));

    // Get the runfile path
    std::string path = runfiles->Rlocation("//example_bin/data:Roboto-Black.ttf");

    std::ifstream file(path,std::ios::in | std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return 1;
    }
    int size=file.tellg();
    file.seekg(4,std::ios::beg);
    char* memdir=new char[size];
    file.read(memdir,size);
    char* end;
    uintmax_t val=strtoimax(memdir,&end,10);
    uint16_t res=(uint16_t)val;
    std::cout<<res;
    file.close();

    return 0;
}