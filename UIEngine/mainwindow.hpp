#include <GLFW/glfw3.h>
#include <string>
using std::string;

class MainWindow{
protected:
    int width;
    int height;
    string name;
    GLFWwindow* handle;
    void onResize(int width,int height);
};