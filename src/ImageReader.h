#ifndef IMAGEREADER
#define IMAGEREADER

#include <vector>

#include "Image.h"
#include "util.h"

/*
 * Container for image reading functions
 */
class ImageReader {
public:
    //read heightmap and equally large colormap to data vector
    static std::vector<Data> read(const Image& heightmap, const Image& colormap, size_t& size, size_t& width, size_t& height);
    //read heightmap to float vector
    static std::vector<float> readHeightmap(const Image& heightmap, size_t& size);

};
#endif