#ifndef IMAGEREADER
#define IMAGEREADER

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Image.h"

class ImageReader {
public:
    struct Data {
        glm::vec4 vertex, normal, color;
    };

    static std::vector<Data> read(Image heightmap, Image colormap);
};
#endif