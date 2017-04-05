#include "Particle.hpp"
#include "../Shader.hpp"
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <vector>

class ParticleSystem {
public:
    ParticleSystem();
    ParticleSystem(glm::vec3 origin, std::string type, int numParticles);
    ~ParticleSystem();

    void init();
    void update(float dt);
    void render(const glm::mat4 &projection, const glm::mat4 &view);
    void createParticles();
    void createShader();
    void createGLBuffers();
    void updateGLData(Particle *p);
    float randNum(float min, float max);
    void setOrigin(glm::vec3 newPos);
    // bool inFrustrum();
    // void collidedWithOrb(const glm::vec3 &playerPos);
    // void setForce(const glm::vec3 &force);
    void debug();

private:
	int id;
    int nParticles;
    std::vector<Particle *> particles;
    std::string particleType;
    glm::vec3 origin;

    GLuint VBO, VAO, EBO;

    std::vector<float> posBuf;
    std::vector<float> colBuf;
    std::vector<float> alpBuf;
    std::vector<float> scaBuf;
    GLuint posBufID;
    GLuint colBufID;
    GLuint alpBufID;
    GLuint scaBufID;

    Shader *shader;

    // GLShaderProgram particleShaderProg;
    // ViewFrustrum frustrum;
};