#ifndef IMAGEREADER
#define IMAGEREADER

#include <vector>

#include "Image.h"
#include "util.h"

class ImageReader {
public:

    static std::vector<Data> read(const Image& heightmap, const Image& colormap);
};
#endif