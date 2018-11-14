#include "Terrain.h"
#include "picopng.hpp"

GLuint Terrain::loadTerrain(GLuint program_id, const char* heightmap_filename) {
    glUseProgram(program_id);
    size_t w, h;
    std::vector<unsigned char> buffer, heightmap;
    loadFile(buffer, heightmap_filename);
    if (decodePNG(heightmap, w, h, &buffer[0], buffer.size(), true)!=0)
        throw std::runtime_error("picopng exception");
    std::vector<float> vertices(512*512*4*6, 1);

    for (size_t z = 0; z < 512; ++z)
        for (size_t x = 0; x < 512; ++x) {
            vertices[(z * 512 + x) * 24 + 0] = x;
            vertices[(z * 512 + x) * 24 + 1] = (float) heightmap[(z * 512 + x)*4];
            vertices[(z * 512 + x) * 24 + 2] = z;
            // vertices[(z * 512 + x) * 12 + 3] = 1;
            vertices[(z * 512 + x) * 24 + 4] = x+1;
            vertices[(z * 512 + x) * 24 + 5] = (float) heightmap[(z * 512 + (x+1))*4];
            vertices[(z * 512 + x) * 24 + 6] = z;
            // vertices[(z * 512 + x) * 12 + 7] = 1;
            vertices[(z * 512 + x) * 24 + 8] = x+1;
            vertices[(z * 512 + x) * 24 + 9] = (float) heightmap[((z+1) * 512 + (x+1))*4];
            vertices[(z * 512 + x) * 24 + 10] = z+1;
            // vertices[(z * 512 + x) * 12 + 11] = 1;
            vertices[(z * 512 + x) * 24 + 12] = x+1;
            vertices[(z * 512 + x) * 24+ 13] = (float) heightmap[((z+1) * 512 + (x+1))*4];
            vertices[(z * 512 + x) * 24 + 14] = z+1;
            // vertices[(z * 512 + x) * 12 + 15] = 1;
            vertices[(z * 512 + x) * 24 + 16] = x;
            vertices[(z * 512 + x) * 24 + 17] = (float) heightmap[((z+1) * 512 + x)*4];
            vertices[(z * 512 + x) * 24 + 18] = z+1;
            // vertices[(z * 512 + x) * 12 + 19] = 1;
            vertices[(z * 512 + x) * 24 + 20] = x;
            vertices[(z * 512 + x) * 24 + 21] = (float) heightmap[(z * 512 + x)*4];
            vertices[(z * 512 + x) * 24 + 22] = z;
            // vertices[(z * 512 + x) * 12 + 23] = 1;
        }

    GLuint heightmapVertexID = 0;
    glGenBuffers(1, &heightmapVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size()*sizeof(float),
        &vertices[0],
        GL_STATIC_DRAW
    );
    GLuint heightmapNormalID = loadNormals(vertices);
    return {heightmapVertexID, heightmapNormalID};
}
 
const static inline vec3 getSurfaceNormal(const vec3& a, const vec3& b, const vec3& c){
  
 vec3 polyVector1 = {b.x - a.x, b.y - a.y, b.z - a.z};
 vec3 polyVector2 = {c.x - a.x, c.y - a.y, c.z - a.z};
  
 vec3 cross = glm::cross(polyVector1, polyVector2);
 return glm::normalize(cross);
}

GLuint Terrain::loadNormals(std::vector<float> vertices) {
    std::vector<float> normals(sizeof(vertices));
    for (size_t i = 0; i < vertices.size(); i+=12) {
        glm::vec3 a = {vertices[i+0], vertices[i+1], vertices[i+2]};
        glm::vec3 b = {vertices[i+4], vertices[i+5], vertices[i+6]};
        glm::vec3 c = {vertices[i+8], vertices[i+9], vertices[i+10]};
        glm::vec3 n = getSurfaceNormal(a, b, c);
        normals[i+0] = n.x;

    }
}