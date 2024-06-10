#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

struct GlyfDescription;
struct Glyf;
struct point;

class FontReader{
    std::ifstream& reader;
public:
    explicit FontReader(std::ifstream& reader):reader(reader){}
    explicit FontReader()=delete;
    ~FontReader(){}
    std::vector<point>        
                ParseFont();
    GlyfDescription        
                ReadGlyfDescription();
    Glyf        ReadGlyf(const GlyfDescription& description);
    int*        ReadCoordinates(uint8_t* flags,bool readingX,int size);
    uint16_t    ReadUInt16();
    uint32_t    ReadUInt32();
    int16_t     ReadInt16();
    uint8_t     ReadByte();
    std::string ReadTag();
    bool        FlagBitIsSet(uint8_t byte,int offset);
    void        SkipBytes(const int count);
    void        GoTo(const int position);
    bool        isLittleEndian();
};

struct GlyfDescription{
    int16_t number_of_contours;
    int16_t xmin;
    int16_t ymin;
    int16_t xmax;
    int16_t ymax;
    friend std::ostream& operator<<(std::ostream& os,GlyfDescription description){
        os<<"GlyphDescription"<<std::endl;
        os<<"Number of Contours: "<<description.number_of_contours<<std::endl;
        os<<"Xmin: "<<description.xmin<<std::endl;
        os<<"Ymin: "<<description.ymin<<std::endl;
        os<<"Xmax: "<<description.xmax<<std::endl;
        os<<"Ymax: "<<description.ymax<<std::endl;
        return os;
    }
};

struct Glyf{
    uint16_t*   endpoints;
    uint16_t   instruction_length;
    uint8_t*    instructions;
    int         variable_size;
    uint8_t*    flags;
    int*    xcoord;
    int*    ycoord;
    friend std::ostream& operator<<(std::ostream& os,const Glyf& glyf){
        int size=sizeof(endpoints)/sizeof(uint16_t);
        os<<"Glyph: "<<'\n';
        os<<"Endpoints of Contours: [ ";
        for(int i=0;i<size+1;i++){
            os<<glyf.endpoints[i]<<" ";
        }
        os<<"]"<<std::endl;
        os<<"Instructions: [ ";
        for(int i=0;i<glyf.instruction_length;i++){
            os<<(int)glyf.instructions[i]<<" ";
        }
        os<<"]"<<std::endl;
        os<<"Flags: [ ";
        for(int i=0;i<glyf.variable_size;i++){
            os<<(int)glyf.flags[i]<<" ";
        }
        os<<"]"<<std::endl;
        os<<"X coordinates: [ ";
        for(int i=0;i<glyf.variable_size;i++){
            os<<(int)glyf.xcoord[i]<<" ";
        }
        os<<"]"<<std::endl;
        os<<"Y coordinates: [ ";
        for(int i=0;i<glyf.variable_size;i++){
            os<<(int)glyf.ycoord[i]<<" ";
        }
        os<<"]"<<std::endl;
        return os;
    }
};


struct point{
    int x;
    int y;
    friend std::ostream& operator<<(std::ostream& os,point p){
        os<<"["<<p.x<<","<<p.y<<"]\n";
        return os;
    }
};
