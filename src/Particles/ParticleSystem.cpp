#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include "../Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <tgmath.h>

#define SHADER_PATH "/Users/ashleydattalo/graphics/LearnOpenGL/src/Shaders/"

static int numPSys = 0;
ParticleSystem::ParticleSystem()
{
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	particleType = "general";
	nParticles = 500;
    id = numPSys++;
    init();
}
    
ParticleSystem::ParticleSystem(glm::vec3 origin, std::string type, int numParticles) :
    origin(origin), particleType(type), nParticles(numParticles) 
    {
        id = numPSys++;
        init();
    }

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::init() {
    posBuf.resize(3*nParticles);
    colBuf.resize(3*nParticles);
    alpBuf.resize(nParticles);
    scaBuf.resize(nParticles);

    createGLBuffers();
    createParticles();
    createShader();
}

void ParticleSystem::update(float dt) {
    for (Particle *p : particles) {
        p->update(dt);
        updateGLData(p);
    }
}

void ParticleSystem::render(const glm::mat4 &projection, const glm::mat4 &view) {
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader->use();

    glBindVertexArray(VAO);

    GLint timeLoc = glGetUniformLocation(shader->getProg(), "time");
    glUniform1f(timeLoc, glfwGetTime());

    GLint projLoc = glGetUniformLocation(shader->getProg(), "P");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint viewLoc = glGetUniformLocation(shader->getProg(), "V");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    //send pos data to gpu
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, alpBufID);
    glBufferData(GL_ARRAY_BUFFER, alpBuf.size()*sizeof(float), &alpBuf[0], GL_STREAM_DRAW);

    // //send color data to gpu
    // glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    // glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_DYNAMIC_DRAW);
    
    glDrawArrays(GL_POINTS, 0, nParticles);

    glBindVertexArray(0);
}

glm::vec3 genPos() {
    static float radius = 1;
    static float x = -radius;
    

    float y = sqrt(radius * radius - x * x);
    std::cout << y << " " << radius << " " << x << std::endl;
    glm::vec3 pos;
    pos.x = x;
    pos.y = -y;

    // radius++;
    x += .1;
    return pos;
}

void ParticleSystem::createParticles() {
	for (int i = 0; i < nParticles; i++) {
        Particle *p = new Particle(i, origin, particleType);
        glm::vec3 col = glm::vec3(randNum(0.5f, 1.0f), randNum(0.5f, 1.0f), randNum(0.5f, 1.0f));
        glm::vec3 pos = glm::vec3(randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f), randNum(0.5f, 1.0f));
        // glm::vec3 pos = genPos();
        float scale = 10;
        float alpha = 1;

        p->setPosition(pos);
        p->setColor(col);
        p->setScale(scale);
        p->setAlpha(alpha);


        particles.push_back(p);
        updateGLData(p);
    }
}

void ParticleSystem::createShader() {
	shader = new Shader(SHADER_PATH "particles.vert", SHADER_PATH "general.frag");
}

void ParticleSystem::createGLBuffers() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glEnable(GL_POINT_SMOOTH);
    GLuint bufs[4];
    glGenBuffers(4, bufs);
    std::cout << "createGLBuffers" << std::endl;
    posBufID = bufs[0];
    colBufID = bufs[1];
    alpBufID = bufs[2];
    scaBufID = bufs[3];

    glGenVertexArrays(1, &VAO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    // Send pos data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


    // Send color buffer to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    // Send scale buffer to GPU
    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glBufferData(GL_ARRAY_BUFFER, scaBuf.size()*sizeof(float), &scaBuf[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);

    // Send alpha buffer to GPU
    glBindBuffer(GL_ARRAY_BUFFER, alpBufID);
    glBufferData(GL_ARRAY_BUFFER, alpBuf.size()*sizeof(float), &alpBuf[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::updateGLData(Particle *p) {
	int index = p->getIndex();
    glm::vec3 col = p->getColor();
    glm::vec3 pos = p->getPosition();

    posBuf[3*index + 0] = pos.x;
    posBuf[3*index + 1] = pos.y;
    posBuf[3*index + 2] = pos.z;

    colBuf[3*index + 0] = col.x;
    colBuf[3*index + 1] = col.y;
    colBuf[3*index + 2] = col.z;

    scaBuf[index] = p->getScale();
    alpBuf[index] = p->getAlpha();

    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), &colBuf[3*index]);

    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), &posBuf[3*index]);

    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(float), sizeof(float), &scaBuf[index]);

    glBindBuffer(GL_ARRAY_BUFFER, alpBufID);
    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(float), sizeof(float), &alpBuf[index]);
}

float ParticleSystem::randNum(float min, float max) {
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * min + r * max;
}

void ParticleSystem::setOrigin(glm::vec3 newPos) {

}

void ParticleSystem::debug() {
    for (Particle *p : particles) {
        std::cout << p->getScale();
        std::cout << "   ";
        std::cout << p->getAlpha();
        std::cout << std::endl;
    }
}
