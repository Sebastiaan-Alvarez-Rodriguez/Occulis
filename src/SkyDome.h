#ifndef SKYDOME
#define SKYDOME

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sun.h"
#include "Camera.h"
#include "preetham.h"
/*Intel:

8.5:
    keep a separate cloud layer, 
    keep a separate stars layer.

7.5: (sun)
    1) full sun : 255,255,220
    2) dawn/dusk, linear fade to : 255, 60, 60
    3) then to : 0, 0, 0
    During 2, spawn a second light source, like the moon, with a blueish tint, like 20, 60, 180, placed 180° opposite to the sun.
    Don't forget to rotate both lights around your scene.
    Some sky gradients may be nice too, from blue to orange to black
    More realistic: search for Mie and Rayleigh scattering functions
*/

//Potential additions:
//Make the dome always have position at camera position
//temporary-turn-off back-face culling

class SkyDome {
public:
    SkyDome(const Sun* sun, const Camera* cam);
    void render(GLenum drawMode, GLuint program_id);
private:
    const Sun* sun;
    const Camera* cam;
    
    glm::vec3 sunDirection;

    GLuint domeVertexID;

    float radius = 2000.0f;
    const size_t rings = 48, sectors = 48;

    PreethamSkyData skyData;


    void setModelView(GLuint p);
    void setSkyData(GLuint p);
    void recalculateSkyData();
    void createSkyDome(size_t r, size_t s);
};
#endif