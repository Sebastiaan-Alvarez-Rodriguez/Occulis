#ifndef TERRAIN
#define TERRAIN
class Terrain {
public:
    GLuint heightmapVertexID, heightmapNormalID;
    GLuint loadTerrain(const char* heightmap_filename);
    GLuint loadNormals();
};
#endif