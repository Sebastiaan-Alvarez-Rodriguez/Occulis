#ifndef IMAGE
#define IMAGE

#include <vector>

class Image {
public:
    struct Pixel {
        float x, y, z, w;
    };
 
    Image(const char* src);
    
    Pixel* data();
    const Pixel& getPixel(size_t x, size_t y);

    size_t width();
    size_t height();
    size_t size();
private:
    size_t w, h;
    std::vector<Pixel> p;
};
#endif