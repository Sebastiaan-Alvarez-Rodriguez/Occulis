#ifndef SKYDOME
#define SKYDOME

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sun.h"
#include "preetham.h"
/*Intel:
7: (dome)
    http://www.rastertek.com/dx11ter07.html
    http://www.rastertek.com/tertut10.html

8.5:
    keep a separate cloud layer, 
    and separate sky-dome with a simple gradient by day, 
    and starry sky by night, (with stars on their own layer.
6:
    https://www.gamedev.net/forums/topic/538429-day--night-cycle-sky-shader-for-you/

7.5: (sun)
    1) full sun : 255,255,220
    2) dawn/dusk, linear fade to : 255, 60, 60
    3) then to : 0, 0, 0
    During 2, spawn a second light source, like the moon, with a blueish tint, like 20, 60, 180, placed 180° opposite to the sun.
    Don't forget to rotate both lights around your scene.
    Some sky gradients may be nice too, from blue to orange to black
    More realistic: search for Mie and Rayleigh scattering functions

8:
    Preetham sky model
*/

//Potential additions:
//Make the dome always have position at camera position
//temporary-turn-off back-face culling

class SkyDome {
public:
    void init(GLuint program_id, const Sun* sun, const Camera* cam);
    void render(GLenum drawMode);
private:
    GLuint program_id;
    GLuint domeVertexID;

    glm::vec3 position;
    glm::mat4 model;

    float radius = 2000.0f;
    const size_t rings = 48, sectors = 48;

    PreethamSkyData skyData;

    const Sun* sun;
    glm::vec3 sunDirection;

    const Camera* cam;

    void setModel();
    void setSkyData();
    void recalculateModel();
    void recalculateSkyData();
    void createSkyDome(size_t r, size_t s);
};
#endif