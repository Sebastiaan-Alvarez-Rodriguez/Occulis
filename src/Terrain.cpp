#include "Terrain.h"
#include "picopng.hpp"

void Terrain::init(GLuint program_id, const char* heightmap_filename) {
    glUseProgram(program_id);
    size_t w, h;
    std::vector<unsigned char> buffer, heightmap;
    loadFile(buffer, heightmap_filename);
    if (decodePNG(heightmap, w, h, &buffer[0], buffer.size(), true)!=0)
        throw std::runtime_error("picopng exception");
    std::vector<glm::vec4> vertices(512*512*6, {0,0,0,1});

    for (size_t z = 0; z < 512; ++z)
        for (size_t x = 0; x < 512; ++x) {
            vertices[(z*512+x)*6+0] = {x,(float)heightmap[(z*512+x)*4],z,1};

            vertices[(z*512+x)*6+1] = {x+1,(float)heightmap[(z*512+(x+1))*4],z,1};

            vertices[(z*512+x)*6+2] = {x+1,(float)heightmap[((z+1)*512+(x+1))*4],z+1,1};

            vertices[(z*512+x)*6+3] = vertices[(z * 512 + x) * 6 + 2];

            vertices[(z*512+x)*6+4] = {x,(float)heightmap[((z+1)*512+x)*4],z+1,1};

            vertices[(z*512+x)*6+5] = vertices[(z*512+x)*6+0];
        }

    heightmapVertexID = 0;
    glGenBuffers(1, &heightmapVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size()*4*sizeof(float),
        &vertices[0],
        GL_STATIC_DRAW
    );
    loadNormals(vertices);
}

void Terrain::render() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapNormalID);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 512*512*6);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}


const static inline glm::vec3 getSurfaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c){
  
 glm::vec3 polyVector1 = {b.x - a.x, b.y - a.y, b.z - a.z};
 glm::vec3 polyVector2 = {c.x - a.x, c.y - a.y, c.z - a.z};
  
 glm::vec3 cross = glm::cross(polyVector1, polyVector2);
 return glm::normalize(cross);
}

void Terrain::loadNormals(std::vector<glm::vec4> vertices) {
    std::vector<glm::vec4> normals(512*512*6);
    for (size_t i = 0; i < vertices.size(); i+=3) {
        glm::vec4 a = {vertices[i+0].x, vertices[i+0].y, vertices[i+0].z, 1};
        glm::vec4 b = {vertices[i+1].x, vertices[i+1].y, vertices[i+1].z, 1};
        glm::vec4 c = {vertices[i+2].x, vertices[i+2].y, vertices[i+2].z, 1};
        glm::vec3 n = getSurfaceNormal(a, b, c);
        normals[i+0] = normals[i+1] = normals[i+2] = {n.x, n.y, n.z, 1};
    }
    heightmapNormalID = 0;
    glGenBuffers(1, &heightmapNormalID);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapNormalID);
    glBufferData(
        GL_ARRAY_BUFFER,
        normals.size()*4*sizeof(float),
        &normals[0],
        GL_STATIC_DRAW
    );
}