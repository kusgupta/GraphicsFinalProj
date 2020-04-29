//
// Created by Kushan Gupta on 4/29/20.
//

#ifndef GLSL_RADIOSITYSCENE_H
#define GLSL_RADIOSITYSCENE_H
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>


class MaterialsCoeffcients {
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 emissive;
    glm::vec3 specular;
    glm::vec3 reflective;
    glm::vec3 transmissive;


};

class Polygon {
    std::vector<glm::vec3> vertices;
};

class Object {
    std::vector<Polygon> polygons;
    MaterialsCoeffcients m;
    glm::vec3 position;
    glm::mat3 rotation;
};

class radiosityScene {
    std::vector<Object> objects;
};

#endif //GLSL_RADIOSITYSCENE_H
