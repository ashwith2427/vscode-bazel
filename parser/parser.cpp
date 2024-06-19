#include "parser.hpp"
#include <stdexcept>

int main(){
    FT_Library library;
    FT_Face face;
    FT_Glyph glyph;
    FT_Error err;

    err=FT_Init_FreeType(&library);
    if(err) throw std::runtime_error("Failed to init");
}