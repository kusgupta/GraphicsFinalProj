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

class Triangle {

public:
    glm::vec4 position[3];
    Triangle(glm::vec4 pos1, glm::vec4 pos2, glm::vec4 pos3) {
        position[0] = pos1;
        position[1] = pos2;
        position[2] = pos3;
    }

    Triangle mdptTri() {
        glm::vec4 m1 = (position[0] + position[1]) / 2.0f;
        glm::vec4 m2 = (position[1] + position[2]) / 2.0f;
        glm::vec4 m3 = (position[2] + position[0]) / 2.0f;

        return Triangle(m1, m2, m3);
    }
};

class SceneObject {
public:
    std::vector<glm::vec4> diffuse;
    glm::vec3 position;
    glm::mat3 rotation;
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;

    void makeTriangles(int level) {
        std::vector<glm::vec4> vertices_copy;
        std::vector<glm::uvec3> faces_copy;

        for (int i = 0; i < vertices.size(); i++) {
            vertices_copy.push_back(vertices[i]);
        }
        for (int i = 0; i < faces.size(); i++) {
            faces_copy.push_back(faces[i]);
        }
        vertices.clear();
        faces.clear();

        for (int i = 0; i < faces_copy.size(); i++) {
            Triangle tri(vertices_copy[faces_copy[i].x], vertices_copy[faces_copy[i].y], vertices_copy[faces_copy[i].z]);
            recursiveTris(level, tri);
        }

    }

    void recursiveTris(int level, Triangle tri) {
        Triangle mid = tri.mdptTri();

        int size = vertices.size();
        if (level == 0) {
            for (int i = 0; i < 3; i++) {
                vertices.push_back(tri.position[i]);
                diffuse.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
            }

            for (int i = 0; i < 3; i++) {
                vertices.push_back(mid.position[i]);
                diffuse.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
            }

            faces.push_back(glm::vec3(size + 3, size + 4, size + 5));
            faces.push_back(glm::vec3(size, size + 3, size + 5));
            faces.push_back(glm::vec3(size + 1, size + 3, size + 4));
            faces.push_back(glm::vec3(size + 2, size + 4, size + 5));
            return;
        }

        else {
            Triangle tri1(tri.position[0], mid.position[0], mid.position[2]);
            Triangle tri2(tri.position[1], mid.position[0], mid.position[1]);
            Triangle tri3(tri.position[2], mid.position[1], mid.position[2]);
            Triangle tri4(mid.position[0], mid.position[1], mid.position[2]);

            recursiveTris(level - 1, tri1);
            recursiveTris(level - 1, tri2);
            recursiveTris(level - 1, tri3);
            recursiveTris(level - 1, tri4);
        }
    }

//    void make_triangles(int num_subdivisions) {
//
//        for (int i = 0; i < num_subdivisions; i++) {
//            for (int j = 0; j < faces.size(); j++)  {
//
//                int v1_pos = faces[j].x;
//                int v2_pos = faces[j].y;
//                int v3_pos = faces[j].z;
//
//                glm::vec4 v1 = vertices[v1_pos];
//                glm::vec4 v2 = vertices[v2_pos];
//                glm::vec4 v3 = vertices[v3_pos];
//
//                // m means midpoint
//                glm::vec4 m1 = (v1 + v2) / 2.0f;
//                glm::vec4 m2 = (v2 + v3) / 2.0f;
//                glm::vec4 m3 = (v3 + v1) / 2.0f;
//
//                int m1_pos = vertices.size();
//                vertices.push_back(m1);
//                int m2_pos = vertices.size();
//                vertices.push_back(m2);
//                int m3_pos = vertices.size();
//                vertices.push_back(m3);
//
//                faces.push_back(glm::vec3(v1_pos, m1_pos, m2_pos));
//                faces.push_back(glm::vec3(m1_pos, v2_pos, m3_pos));
//                faces.push_back(glm::vec3())
//                // TODO: REMOVE THE ORIGINAL FACE
//
//            }
//        }
//
//    }


};

class radiosityScene {
    std::vector<SceneObject> objects;
};

#endif //GLSL_RADIOSITYSCENE_H
