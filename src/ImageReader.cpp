#include "ImageReader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const static inline glm::vec4 readVertexPixel(const Image::Pixel& p, size_t x, size_t z) {
    return {x, p.y, z, 1};
}

const static inline glm::vec4 readColorPixel(const Image::Pixel& p) {
    return {p.x, p.y, p.z, p.w};
}

const static inline glm::vec4 getSurfaceNormal(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c){
  
 glm::vec3 polyVector1 = {b.x - a.x, b.y - a.y, b.z - a.z};
 glm::vec3 polyVector2 = {c.x - a.x, c.y - a.y, c.z - a.z};
  
 glm::vec3 cross = glm::cross(polyVector1, polyVector2);
 return {glm::normalize(cross),1};
}

std::vector<Data> ImageReader::read(const Image& heightmap, const Image& colormap, size_t& size, size_t& width, size_t& height) {
    std::vector<Data> ret;
    size_t w = heightmap.width(), h = heightmap.height();
    width = w, height = h;
    size = (w-1)*(h-1)*6;
    ret.reserve(size);
    for (size_t z = 0; z < h-1; ++z)
        for (size_t x = 0; x < w-1; ++x) {
            ret.push_back({
                readVertexPixel(heightmap.getPixel(x, z), x, z), 
                {0,0,0,0}, 
                readColorPixel(colormap.getPixel(x, z))
            });
            ret.push_back({
                readVertexPixel(heightmap.getPixel(x+1, z+1), x+1, z+1), 
                {0,0,0,0}, 
                readColorPixel(colormap.getPixel(x+1, z+1))
            });
            ret.push_back({
                readVertexPixel(heightmap.getPixel(x+1, z), x+1, z), 
                {0,0,0,0}, 
                readColorPixel(colormap.getPixel(x+1, z))
            });
            ret.push_back({
                readVertexPixel(heightmap.getPixel(x+1, z+1), x+1, z+1), 
                {0,0,0,0}, 
                readColorPixel(colormap.getPixel(x+1, z+1))
            });
            ret.push_back({
                readVertexPixel(heightmap.getPixel(x, z), x, z), 
                {0,0,0,0}, 
                readColorPixel(colormap.getPixel(x, z))
            });
            ret.push_back({
                readVertexPixel(heightmap.getPixel(x, z+1), x, z+1), 
                {0,0,0,0}, 
                readColorPixel(colormap.getPixel(x, z+1))
            });
        }

    for (size_t i = 0; i < ret.size(); i+=3) {
        ret[i].normal = ret[i+1].normal = ret[i+2].normal = 
            getSurfaceNormal(ret[i].vertex, ret[i+1].vertex, ret[i+2].vertex);
    }
    return ret;
}

std::vector<float> ImageReader::readHeightmap(const Image& heightmap, size_t& size) {
    std::vector<float> ret;
    size_t w = heightmap.width(), h = heightmap.height();
    size = (w-1)*(h-1);
    ret.reserve(size);
    for (size_t z = 0; z < h-1; ++z)
        for (size_t x = 0; x < w-1; ++x)
            ret.push_back(heightmap.getPixel(x, z).x);
    return ret;
}