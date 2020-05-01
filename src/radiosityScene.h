//
// Created by Kushan Gupta on 4/29/20.
//

#ifndef GLSL_RADIOSITYSCENE_H
#define GLSL_RADIOSITYSCENE_H
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>


class LightSource {
public:
    glm::vec4 position;
    glm::vec4 intensity;
    glm::vec4 color;
};

class SceneObject {
public:
    std::vector<glm::vec4> diffuse;
    glm::vec3 position;
    glm::mat3 rotation;
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
};

class radiosityScene {
    std::vector<SceneObject> objects;
};

#endif //GLSL_RADIOSITYSCENE_H
