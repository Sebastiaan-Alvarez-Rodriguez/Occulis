#include "Self.h"
#include "shader.hpp"
#include "picopng.hpp"
#include "error.hpp"

Self::Self(inputstate& i) : in(i) {
    programID = LoadShaders("_shaderV.c", "_shaderF.c");
    glUseProgram(programID);
    //create vbo
    glGenBuffers(1, &squareVertexID);
    //then bind it, so that the next operation is performed on this object
    glBindBuffer(GL_ARRAY_BUFFER, squareVertexID);
    float squareVertices[] = {
        -10, -10, 0,
        -10,  10, 0,
         10, -10, 0,

         10, -10, 0,
         10,  10, 0,
        -10,  10, 0
    };

    //send the data to the GPU
    glBufferData(
        GL_ARRAY_BUFFER,
        6*3*sizeof(float),                //size of data in bytes
        &squareVertices[0],               //pointer to start of data
        GL_STATIC_DRAW                    //expected method of updating the data
    );

    glGenBuffers(1, &squareColorID);
    //then bind it, so that the next operation is performed on this object
    glBindBuffer(GL_ARRAY_BUFFER, squareColorID);
    float squareColor[] = {
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1,

        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
    };

    //send the data to the GPU
    glBufferData(
        GL_ARRAY_BUFFER,
        6*4*sizeof(float),                //size of data in bytes
        &squareColor[0],                  //pointer to start of data
        GL_STATIC_DRAW                    //expected method of updating the data
    );
    size_t w, h;
    std::vector<unsigned char> buffer, heightmap;
    loadFile(buffer, "terrain_heightmap.png");
    if (decodePNG(heightmap, w, h, &buffer[0], buffer.size(), false)!=0)
        throw std::runtime_error("picpng exception");
    if (errCheck())
        throw std::runtime_error("gl_exception");
}

void Self::update(int width, int height, double deltatime) {

}

void Self::render() {
    glBindBuffer(GL_ARRAY_BUFFER, squareVertexID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, squareColorID);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    if (errCheck())
        throw std::runtime_error("gl_exception");
}