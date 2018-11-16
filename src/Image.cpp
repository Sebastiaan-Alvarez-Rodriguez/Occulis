#include "Image.h"
#include "picopng.hpp"

Image::Image(const char* src) {
    std::vector<unsigned char> buffer, map;
    loadFile(buffer, src);
    if (decodePNG(map, w, h, &buffer[0], buffer.size(), true)!=0)
        throw std::runtime_error("picopng exception");

    for (size_t i = 0; i < map.size(); i+=4)
        p.push_back({(float)map[i],(float)map[i+1],(float)map[i+2],(float)map[i+3]});
}


const Image::Pixel& Image::getPixel(size_t x, size_t y) {
    return p[y*w+x];
}

Image::Pixel* Image::data() {
    return p.data();
}

size_t Image::width() {
    return w;
}

size_t Image::height() {
    return h;
}

size_t Image::size() {
    return p.size();
}