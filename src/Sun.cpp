#include "Sun.h"

#include <stdexcept>

void Sun::init(GLuint program_id) {
    glUseProgram(program_id);
    createSphere(48, 48);
    position = {512, 1000, 512};
}

void Sun::createSphere(size_t rings, size_t sectors) {
    if (rings < 1 || sectors < 1)
        throw std::runtime_error("Sphere created with invalid parameters");
    const float R = 1./(float)(rings-1);
    const float S = 1./(float)(sectors-1);

    std::vector<glm::vec4> vertices(rings * sectors * 6, {0,0,0,1});
    std::vector<glm::vec4> normals(rings * sectors * 6, {0,0,0,1});
    std::vector<glm::vec4> colors(rings * sectors * 6, {253,184,19,255});
    for(size_t r = 0; r < rings; ++r) 
        for(size_t s = 0; s < sectors; ++s) {
            const float x_0=cos(2*M_PI*s*S)*sin(M_PI*r*R);
            const float y_0=sin(-M_PI_2+M_PI*r*R);
            const float z_0=sin(2*M_PI*s*S)*sin(M_PI*r*R);

            const float x_1=cos(2*M_PI*(s+1)*S)*sin(M_PI*r*R);
            const float y_1=y_0;
            const float z_1=sin(2*M_PI*(s+1)*S)*sin(M_PI*r*R);

            const float x_2=cos(2*M_PI*(s+1)*S)*sin(M_PI*(r+1)*R);
            const float y_2=sin(-M_PI_2+M_PI*(r+1)*R);
            const float z_2=sin(2*M_PI*(s+1)*S)*sin(M_PI*(r+1)*R);

            const float x_3=cos(2*M_PI*s*S)*sin(M_PI*(r+1)*R);
            const float y_3=sin(-M_PI_2+M_PI*(r+1)*R);
            const float z_3=sin(2*M_PI*s*S)*sin(M_PI*(r+1)*R);

            vertices[(r*rings+s)*6+0] = {x_0*radius,y_0*radius,z_0*radius,1};
            vertices[(r*rings+s)*6+1] = {x_1*radius,y_1*radius,z_1*radius,1};
            vertices[(r*rings+s)*6+2] = {x_2*radius,y_2*radius,z_2*radius,1};
            vertices[(r*rings+s)*6+3] = vertices[(r*rings+s)*6+2];
            vertices[(r*rings+s)*6+4] = {x_3*radius,y_3*radius,z_3*radius,1};
            vertices[(r*rings+s)*6+5] = vertices[(r*rings+s)*6+0];

            normals[(r*rings+s)*6+0] = {x_0,y_0,z_0,1};
            normals[(r*rings+s)*6+1] = {x_1,y_1,z_1,1};
            normals[(r*rings+s)*6+2] = {x_2,y_2,z_2,1};
            normals[(r*rings+s)*6+3] = normals[(r*rings+s)*6+2];
            normals[(r*rings+s)*6+4] = {x_3,y_3,z_3,1};
            normals[(r*rings+s)*6+5] = normals[(r*rings+s)*6+0];
    }
    sunVertexID = 0;
    glGenBuffers(1, &sunVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, sunVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size()*4*sizeof(float),
        &vertices[0],
        GL_STATIC_DRAW
    );
    sunNormalID = 0;
    glGenBuffers(1, &sunNormalID);
    glBindBuffer(GL_ARRAY_BUFFER, sunNormalID);
    glBufferData(
        GL_ARRAY_BUFFER,
        normals.size()*4*sizeof(float),
        &normals[0],
        GL_STATIC_DRAW
    );
    glGenBuffers(1, &sunColorID);
    glBindBuffer(GL_ARRAY_BUFFER, sunColorID);
    glBufferData(
        GL_ARRAY_BUFFER,
        colors.size()*4*sizeof(float),
        &colors[0],
        GL_STATIC_DRAW
    );
}

void Sun::render() {
    //enable vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, sunVertexID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //enable normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, sunNormalID);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //enable colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, sunColorID);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 48*48*6);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}