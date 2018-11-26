#ifndef IMAGEREADER
#define IMAGEREADER

#include <vector>

#include "Image.h"
#include "util.h"

class ImageReader {
public:

    static std::vector<Data> read(const Image& heightmap, const Image& colormap, size_t& size, size_t& width, size_t& height);
    static std::vector<float> readHeightmap(const Image& heightmap, size_t& size);

};
#endif