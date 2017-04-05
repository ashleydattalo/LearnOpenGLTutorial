#include "Particle.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

#define PI 3.14

Particle::Particle() {
    init();
}

Particle::Particle(int index, glm::vec3 origin, std::string type) :
    index(index), origin(origin), type(type) {
        init();
    }

Particle::Particle(glm::vec3 position, glm::vec3 color, float scale, float alpha) :
    position(position), color(color), scale(scale), alpha(alpha) {
        init();
    }

Particle::~Particle() {}

void Particle::init() {
    color = glm::vec3(randFloat(0.5f, 1.0f), randFloat(0.5f, 1.0f), randFloat(0.5f, 1.0f));
    position = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(0.5f, 1.0f));
    scale = randFloat(1, 5);
    alpha = 1.0f;
    t = randFloat(-10, 10);
    rebirth(t);
}

void Particle::rebirth(float dt) {
    mass = 10.0f;
    lifespan = 2.0f;
    tEnd = t + lifespan;
}

void Particle::update(float dt) {
    static float angle = 0;
    t += dt * .06;
    // t *= .01;
    if (t > tEnd) {
        rebirth(t);
    }
    alpha = (tEnd-t)/lifespan;
    std::cout << alpha << std::endl;
    float radius = 3;
    float period = 1.5;

    radius = glm::sin(t * period);
    position.x = radius * glm::cos(t * period);
    position.y = fmod(t, 360)/4;
    position.z = radius * glm::sin(t * period);
    // position += glm::vec3(origin.x, origin.y, origin.z);

    glm::mat4 transform;
    glm::vec3 rotationAxis(1.0f, 0.0f, 0.0f);

    transform = glm::translate(transform, glm::vec3(position.x, position.x, position.z));
    transform = glm::rotate(transform, 30.0f, rotationAxis);
    position = glm::vec3(transform * glm::vec4(position, 1.0f));
}


void Particle::setOrigin(glm::vec3 newOrig) {
    this->origin = newOrig;
}

float Particle::randFloat(float min, float max) {
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * min + r * max;
}

void Particle::updatePosition(glm::vec3 update) {
    this->position += update;
}

void Particle::setPosition(glm::vec3 pos) {
    this->position = pos;
}
void Particle::setColor(glm::vec3 col) {
    this->color = col;
}
void Particle::setScale(float sca) {
    this->scale = sca;
}
void Particle::setAlpha(float alp) {
    this->alpha = alp;
}


int Particle::getIndex() {
    return this->index;
}

glm::vec3 Particle::getPosition() {
    return this->position;
}
glm::vec3 Particle::getColor() {
    return this->color;
}

float Particle::getScale() {
    return this->scale;
}

float Particle::getAlpha() {
    return this->alpha;
}
