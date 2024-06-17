#include "parser.hpp"
#include <bit>
#include <cstdint>
#include <ios>
#include <string>
#include <map>

std::vector<point> FontReader::ParseFont(){
    std::map<std::string,int> lookup;
    SkipBytes(4);
    uint16_t tables=ReadUInt16();
    SkipBytes(6);
    for(int i=0;i<tables;i++){
        std::string tag=ReadTag();
        uint32_t checksum=ReadUInt32();
        uint32_t offset=ReadUInt32();
        uint32_t length=ReadUInt32();
        lookup[tag]=offset;
    }
    GoTo(lookup["glyf"]);
    GlyfDescription description=ReadGlyfDescription();
    Glyf glyf=ReadGlyf(description);

    std::vector<point> points;
    for(int i=0;i<glyf.variable_size;i++){
        points.push_back(point{glyf.xcoord[i],glyf.ycoord[i]});
    }
    return points;
}

Glyf FontReader::ReadGlyf(const GlyfDescription& description){
    Glyf glyf{};
    glyf.endpoints=new uint16_t[description.number_of_contours];
    for(int i=0;i<description.number_of_contours;i++){
        glyf.endpoints[i]=ReadUInt16();
    }
    glyf.instruction_length=ReadUInt16();
    glyf.instructions=new uint8_t[glyf.instruction_length];
    for(int i=0;i<glyf.instruction_length;i++){
        glyf.instructions[i]=ReadByte();
    }
    glyf.variable_size=glyf.endpoints[description.number_of_contours-1]+1;
    int length=glyf.variable_size;
    glyf.flags=new uint8_t[length];
    for(int i=0;i<length;i++){
        uint8_t flag=ReadByte();
        glyf.flags[i]=flag;
        if(FlagBitIsSet(flag, 3)){
            for(int j=0;j<ReadByte();j++){
                glyf.flags[++i]=flag;
            }
        }
    }
    glyf.xcoord=ReadCoordinates(glyf.flags,true,length);
    glyf.ycoord=ReadCoordinates(glyf.flags, false, length);

    return glyf;
}

int* FontReader::ReadCoordinates(uint8_t* flags,bool readingX,int size){
    int offsetSizeFlagBit = readingX ? 1 : 2;
    int offsetSignOrSkipBit = readingX ? 4 : 5;
    int* coordinates = new int[size];

    for (int i = 0; i < size; i++)
    {
        coordinates[i] = coordinates[std::max(0, i - 1)];
        uint8_t flag = flags[i];
        bool onCurve = FlagBitIsSet(flag, 0);
        if (FlagBitIsSet(flag, offsetSizeFlagBit))
        {
            uint8_t offset = ReadByte();
            int sign = FlagBitIsSet(flag, offsetSignOrSkipBit) ? 1 : -1;
            coordinates[i] += offset * sign;
        }
        else if (!FlagBitIsSet(flag, offsetSignOrSkipBit))
        {
            coordinates[i] += ReadInt16();
        }
    } 
    return coordinates;
}

GlyfDescription FontReader::ReadGlyfDescription(){
    int16_t number_of_contours=ReadInt16();
    int16_t xmin=ReadInt16();
    int16_t ymin=ReadInt16();
    int16_t xmax=ReadInt16();
    int16_t ymax=ReadInt16();
    return GlyfDescription{
        number_of_contours,
        xmin,
        ymin,
        xmax,
        ymax
    };
}

bool FontReader::isLittleEndian(){
    return std::endian::little==std::endian::native;
}

uint16_t FontReader::ReadUInt16(){
    uint16_t value;
    reader.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!isLittleEndian()){
        return value;
    }
    return (value<<8)|(value>>8);
}

uint32_t FontReader::ReadUInt32(){
    uint32_t value;
    reader.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!isLittleEndian()){
        return value;
    }
    uint8_t mask=0xFF;
    uint32_t A=value&mask;
    uint32_t B=(value>>8)&mask;
    uint32_t C=(value>>16)&mask;
    uint32_t D=(value>>24)&mask;
    value=(A<<24)|(B<<16)|(C<<8)|D;
    return value;
}

int16_t FontReader::ReadInt16(){
    int16_t value;
    reader.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!isLittleEndian()){
        return value;
    }
    int16_t mask=0xFF;
    int16_t A= value&mask;
    int16_t B=(value>>8)&mask;
    value=(A<<8)|B;
    return value;
}

std::string FontReader::ReadTag(){
    char* tag=new char[4];
    reader.read(tag, 4);
    std::string res="";
    for(int i=0;i<4;i++){
        res+=(char)tag[i];
    }
    return res;
}

uint8_t FontReader::ReadByte(){
    uint8_t value;
    reader.read(reinterpret_cast<char*>(&value),sizeof(value));
    return (int)value;
}

bool FontReader::FlagBitIsSet(uint8_t byte,int offset){
    return ((byte>>offset)&1)==1;
}

void FontReader::SkipBytes(const int count){
    reader.seekg(count,std::ios::cur);
}

void FontReader::GoTo(const int position){
    reader.seekg(position);
}
