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
    glm::vec4 intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 color;
    bool used = false;
};

class Triangle {

public:
    glm::vec4 position[3];
    glm::vec4 diffuse_constant = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 diffuse_lighting;
    glm::vec4 accum = glm::vec4(0.0, 0.0, 0.0, 0.0);
    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 0.0);

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

    glm::vec3 normal() {
        glm::vec3 s1 = position[1] - position[0];
        glm::vec3 s2 = position[2] - position[0];
        return glm::normalize(glm::cross(s1, s2));
    }

    glm::vec3 centroid() {
        return (position[0] + position[1] + position[2]) / 3.0f;
    }

    bool intersects(glm::vec3 direction, glm::vec3 pos) {
        glm::vec3 n = normal();

        float t = glm::dot(glm::vec3(position[0]) - pos, n) / glm::dot(direction, n);

        if (t < 0) {
            return false;
        }


        glm::vec3 intersection = pos + direction * t;

        for (int i = 0; i < 3; i++) {
            glm::vec3 s1 = position[(i + 1) % 3] - position[i];
            glm::vec3 s2 = position[(i + 2) % 3] - position[i];

            if (glm::dot(glm::cross(s1, s2), glm::cross(intersection - glm::vec3(position[i]), s2)) < 0) {
                return false;
            }
        }

        return true;
    }
};

class SceneObject {
public:
    std::vector<glm::vec4> colors;
    glm::vec3 position;
    glm::mat3 rotation;
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<Triangle> triangles;

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
        colors.clear();
        for (int i = 0; i < faces_copy.size(); i++) {
            Triangle tri(vertices_copy[faces_copy[i].x], vertices_copy[faces_copy[i].y],
                         vertices_copy[faces_copy[i].z]);
            recursiveTris(level, tri);
        }

    }

    void recursiveTris(int level, Triangle tri) {
        Triangle mid = tri.mdptTri();

        int size = vertices.size();
        if (level == 0) {
            int tri_size = triangles.size();
            vertices.push_back(tri.position[0]);
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(tri.position[0], mid.position[0], mid.position[2]));
            vertices.push_back(tri.position[1]);
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[1]);
            triangles.push_back(Triangle(tri.position[1], mid.position[0], mid.position[1]));
            vertices.push_back(tri.position[2]);
            vertices.push_back(mid.position[1]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(tri.position[2], mid.position[1], mid.position[2]));
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[1]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(mid.position[0], mid.position[1], mid.position[2]));

            for (int i = 0; i < 4; i++) {
                triangles[triangles.size() - 4 + i].diffuse_constant = glm::vec4(1.0, 1.0, 1.0, 1.0);
            }
            for (int i = 0; i < 12; i++) {
//                if (i % 2 == 0)
                    colors.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));
//                else
//                    diffuse.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
            }
            faces.push_back(glm::vec3(size , size + 1, size + 2));
            faces.push_back(glm::vec3(size + 3, size + 4, size + 5));
            faces.push_back(glm::vec3(size + 6, size + 7, size + 8));
            faces.push_back(glm::vec3(size + 9, size + 10, size + 11));
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

    void updateColors() {
        int list_length = vertices.size();
        colors.clear();
        for (int i = 0; i < list_length; i++) {
            colors.push_back(glm::vec4(0, 0, 0, 1));
        }
        for (int i = 0; i < triangles.size(); i++) {
            Triangle tri = triangles[i];
            for (int pos = 0; pos < 3; pos++) {
                colors[3 * i + pos].x = triangles[i].color.x;
                colors[3 * i + pos].y = triangles[i].color.y;
                colors[3 * i + pos].z = triangles[i].color.z;
            }
        }
    }

    float attenuation(float distance) {
        float c = .10f;
        return 1 / (c + c * distance);// + c * distance * distance);
//        return 1.0f;
    }

    std::vector<Triangle> unobstructed_triangles(Triangle *tri1) {
        std::vector<Triangle> unobstructed;
        for (int i = 0; i < triangles.size(); i++) {
            Triangle tri2 = triangles[i];
            glm::vec3 direction = glm::normalize(tri2.centroid() - tri1->centroid());
            float distance = glm::distance(tri1->centroid(), tri2.centroid());
            bool intersected = false;
            for (int j = 0; j < triangles.size(); j++) {
                if (j == i) {
                    continue;
                }
                if (triangles[j].intersects(tri1->centroid(), direction)){
                    intersected = true;
                }
            }
            if (!intersected) {
                unobstructed.push_back(tri2);
            }
        }
        return unobstructed;
    }

    void calculate_light(LightSource lightSource) {
        //Leftover light is initially 0
        //Leftover accumulates as it gets light
        //Diffuse is the amount of light to diffuse currently
        //Vertices will contain the accumulated light
        if (!lightSource.used) {
            for (int i = 0; i < triangles.size(); i++) {
                Triangle *tri = &triangles[i];
                glm::vec3 normal = tri->normal();

                glm::vec4 centroid(0.0, 0.0, 0.0, 0.0);

                for (int i = 0; i < 3; i++) {
                    centroid += tri->position[i];
                }

                centroid /= 3.0;

                glm::vec3 lightDir = glm::normalize(lightSource.position - centroid);
                float distance = glm::distance(lightSource.position, centroid) / 80;
                tri->diffuse_lighting = tri->diffuse_constant * attenuation(distance) * lightSource.intensity * glm::abs(glm::dot(lightDir, normal));
                tri->color = tri->diffuse_lighting;

//                tri->color = glm::vec4(1.0f,1.0f, 1.0f, 1.0f);
            }

            lightSource.used = true;
        } else {
            for (int i = 0; i < triangles.size(); i++) {
                Triangle *tri1 = &triangles[i];
                // TODO: Create unobstructed triangle list for the next for loop
                std::vector<Triangle> unobstructed = unobstructed_triangles(tri1);
                for (int j = 0; j < unobstructed.size(); j++) {
                    if (j == i) {
                        continue;
                    }

                    Triangle *tri2 = &unobstructed[i];

                    glm::vec3 c1 = tri1->centroid();
                    glm::vec3 c2 = tri2->centroid();

                    glm::vec3 n1 = tri1->normal();
                    glm::vec3 n2 = tri2->normal();


                    glm::vec3 cDist = c2 - c1;
                    float distance = glm::length(cDist) / 80;
                    cDist = glm::normalize(cDist);
                    float angle1 = glm::acos(glm::dot(n1, cDist));
                    float angle2 = glm::acos(glm::dot(n2, cDist));



                    float form_factor = glm::abs(glm::cos(angle1) * glm::cos(angle2) / (3.14 * distance * distance));
                    tri1->accum += (form_factor * tri2->diffuse_lighting) / (triangles.size() * 1.0f);
                }

            }

            for (int i = 0; i < triangles.size(); i++) {
                triangles[i].diffuse_lighting = triangles[i].diffuse_constant * triangles[i].accum;
                triangles[i].color += triangles[i].diffuse_lighting;
                triangles[i].accum = glm::vec4(0.0, 0.0, 0.0, 0.0);
            }
        }
    }

};

class radiosityScene {
    std::vector<SceneObject> objects;
};

#endif //GLSL_RADIOSITYSCENE_H
