#ifndef IMAGE
#define IMAGE

#include <cstddef>
#include <vector>

class Image {
public:
    struct Pixel {
        float x, y, z, w;
    };
 
    Image(const char* src);
    
    Pixel* data();
    const Pixel& getPixel(size_t x, size_t y) const;

    size_t width() const;
    size_t height() const;
    size_t size() const;
private:
    size_t w, h;
    std::vector<Pixel> p;
};
#endif