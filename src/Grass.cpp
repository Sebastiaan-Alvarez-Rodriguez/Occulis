#include "Grass.h"

#include "util.h"

// https://github.com/dbaranchuk/grass-simulator-opengl/blob/master/GrassSimulator/Utility/src/main.cpp

Grass::Grass(const Camera* cam, const Wind* wind, const Sun* sun, const Ground* ground, glm::vec2 pos, float radius, size_t amt): 
    cam(cam), wind(wind), sun(sun), ground(ground), position({pos.x-radius, 0, pos.y-radius, 1}), radius(radius), instances(amt) {
        
    auto a = genMesh();//main:400

    glGenBuffers(1, &pointsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        a.size()*sizeof(glm::vec4),
        &a[0],
        GL_STATIC_DRAW
    );

    /*positions*/
    generatePositions();
    glGenBuffers(1, &grassPositions);
    glBindBuffer(GL_ARRAY_BUFFER, grassPositions);
    glBufferData(
        GL_ARRAY_BUFFER, 
        data.positions.size()*sizeof(glm::vec4), 
        &data.positions[0],
        GL_STATIC_DRAW
    );

    /*rotations*/
    generateRotations();
    glGenBuffers(1, &grassRotations);
    glBindBuffer(GL_ARRAY_BUFFER, grassRotations);
    glBufferData(
        GL_ARRAY_BUFFER, 
        data.rotations.size()*sizeof(glm::vec2), 
        &data.rotations[0],
        GL_STATIC_DRAW
    );    

    /*scales*/
    generateScales();
    glGenBuffers(1, &grassScales);
    glBindBuffer(GL_ARRAY_BUFFER, grassScales);
    glBufferData(
        GL_ARRAY_BUFFER, 
        data.scales.size()*sizeof(float), 
        data.scales.data(),
        GL_STATIC_DRAW
    ); 

    /*colors*/
    generateColors();
    glGenBuffers(1, &grassColor);
    glBindBuffer(GL_ARRAY_BUFFER, grassColor);
    glBufferData(
        GL_ARRAY_BUFFER,
        data.colors.size()*sizeof(glm::vec4),
        &data.colors[0],
        GL_STATIC_DRAW
    );

    //main:484 texture generation skipped. important?
}

void Grass::render(GLenum drawMode, GLuint program_id) {
    setModelView(program_id);
    setWData(program_id);
    setSunData(program_id);
    //enable instance
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);//main:433

    //enable positions
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, grassPositions);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);//main:441
    glVertexAttribDivisor(1, 1);

    //enable rotations
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, grassRotations);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);//main:441
    glVertexAttribDivisor(2, 1);

    //enable scales
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, grassScales);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);//main:441
    glVertexAttribDivisor(3, 1);

    //enable colors
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, grassColor);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribDivisor(4, 1);

    glDrawArraysInstanced(drawMode, 0, genMesh().size(), instances);

    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

std::vector<glm::vec4> Grass::genMesh() {
    return {
        {0, 0, 0, 1},
        {0.8, 0, 0, 1},
        {0.4, 0.3, 0, 1},

        {0.4, 0.3, 0, 1},
        {1.2, 0.3, 0, 1},
        {0.8, 0, 0, 1},

        {0.4, 0.3, 0, 1},
        {1.2, 0.3, 0, 1},
        {1, 0.6, 0, 1},

        {1, 0.6, 0, 1},
        {1.6, 0.6, 0, 1},
        {1.2, 0.3, 0, 1},

        {1, 0.6, 0, 1},
        {1.6, 0.6, 0, 1},
        {2, 1, 0, 1},
    };
}

void Grass::generatePositions() {
    for (size_t i = 0; i < instances; ++i) {
        size_t x, y, z;
        do {
        x = randuint(0, 2*radius);
        z = randuint(0, 2*radius);
        y = ground->getHeightAt(x+position.x, z+position.z);
        } while (y > 70.0f);

        data.positions.push_back({x, y, z, 1});
    }
}

void Grass::generateRotations() {
    for (size_t i = 0; i < instances; ++i) {
        float angle = float(rand()) / RAND_MAX * M_PI;
        data.rotations.push_back({cos(angle), sin(angle)});
    }
}

void Grass::generateScales() {
    for (size_t i = 0; i < instances; ++i)
        data.scales.push_back((float) ((float)rand()/(float)RAND_MAX));
}

void Grass::generateColors() {
    for (size_t i = 0; i < instances; ++i)
        data.colors.push_back({0.7, 0.8 - ((float)rand()/RAND_MAX)/5, 0.01, 1.0});
}

void Grass::setModelView(GLuint program_id) {
    glm::mat4 Model = glm::translate(glm::mat4(1.0f), {position.x, 0, position.z});
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "model"), 
        1,
        GL_FALSE,
        &Model[0][0]
    );
    glm::mat4 view = cam->getView();
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "view"), 
        1,
        GL_FALSE,
        &view[0][0]
    );
}

void Grass::setWData(GLuint program_id) {
    Wind::WData a = wind->getWData();
    glUniform4f(
        glGetUniformLocation(program_id, "direction"), 
        a.direction.x, a.direction.y, a.direction.z, a.direction.w
    );

    glUniform1f(
        glGetUniformLocation(program_id, "strength"), 
        a.strength
    );
    glUniform1f(
        glGetUniformLocation(program_id, "baseAngle"), 
        a.baseAngle
    );
    
}

void Grass::setSunData(GLuint p) {
    glm::vec3 sunLoc = sun->getPosition();
    glUniform3f(glGetUniformLocation(p, "sunLoc"), sunLoc.x, sunLoc.y, sunLoc.z);
    glm::vec3 lightColor = sun->getLightColor();
    glUniform3f(glGetUniformLocation(p, "sunColor"), lightColor.x, lightColor.y, lightColor.z);
}