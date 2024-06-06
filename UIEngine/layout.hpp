#include "mainwindow.hpp"
#include "robject.hpp"
#include <vector>
using std::vector;

class Layout:public MainWindow{
    int id;
    vector<RObject> children;
    Color backgroundColor;
};

class Column:public Layout{
    int maxWidth;
    int maxHeight;
};