#include <tuple>
using std::tuple;

using Color=tuple<int,int,int,int>;

class Side{
    int top;
    int bottom;
    int left;
    int right;
    int all;
    int horizontal;
    int vertical;
};

class Padding:public Side{};
class Spacing:public Side{};

enum Alignment{
    Center,
    Start,
    End
};

class RObject{
    int id;
    Color color;
    Padding padding;
    Spacing spacing;
    Alignment horizontalAlignment;
    Alignment verticalAlignment;
public:
    void onHover();
    void onClick();
    void onUpdate();
    void signal(void* data);
    void getObjectId();
private:
    void applyColor();
    void applyConstraints();    
};