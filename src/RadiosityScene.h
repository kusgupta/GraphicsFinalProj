#pragma once
//
// Created by Kushan Gupta on 4/29/20.
//

#ifndef GLSL_RADIOSITYSCENE_H
#define GLSL_RADIOSITYSCENE_H

#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>
#include <thread>
#include <map>
#include <algorithm>
#include "OBJ_Loader.h"
//#include "kdTree.h"
#include "bbox.h"

//class kdTre


class LightSource {
public:
    glm::vec4 position;
    glm::vec4 intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool used = false;
};

class AreaLight {
public:
    std::vector<LightSource> lights;

    float computeArea() {
//        float s1 = glm::length(lights[0] )
    }
};


class Triangle {

public:
    glm::vec4 position[3];
    glm::vec4 vertex_color[3];
    glm::vec4 diffuse_constant = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    int index;
//    glm::vec4
    glm::vec4 diffuse_lighting;
    glm::vec4 accum = glm::vec4(0.0, 0.0, 0.0, 0.0);
    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 0.0);
//    float *form_factors;
    std::map<int, float> form_factors;
    kdBox box;

    Triangle(glm::vec4 pos1, glm::vec4 pos2, glm::vec4 pos3, glm::vec4 color1, glm::vec4 color2, glm::vec4 color3,
             int face_index) {
        position[0] = pos1;
        position[1] = pos2;
        position[2] = pos3;
        vertex_color[0] = color1;
        vertex_color[1] = color2;
        vertex_color[2] = color3;
        diffuse_constant = (color1 + color2 + color3) / 3.0f;
        index = face_index;
        glm::vec3 min_box(10000000.0, 10000000.0, 10000000.0);
        glm::vec3 max_box(-10000000.0, -10000000.0, -100000000.0);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (position[i][j] < min_box[j]) {
                    min_box[j] = position[i][j];
                }

                if (position[i][j] > max_box[j]) {
                    max_box[j] = position[i][j];
                }
            }
        }

//        std::cout << "Positions: " << glm::to_string(position[0]) << " " << glm::to_string(position[1]) << " " << glm::to_string(position[2]) << std::endl;
//        std::cout << "Min: " << glm::to_string(min_box) << " Max: " << glm::to_string(max_box) << std::endl;
        kdBox tempbox(min_box, max_box);


        box = tempbox;
    }

    const kdBox getBoundingBox() {
        return box;
    }

    float getArea() {
        glm::vec3 s1 = position[0] - position[1];
        glm::vec3 s2 = position[1] - position[2];
        return glm::length(glm::cross(s1, s2)) / 2;
    }

    Triangle mdptTri() {
        glm::vec4 m1 = (position[0] + position[1]) / 2.0f;
        glm::vec4 m2 = (position[1] + position[2]) / 2.0f;
        glm::vec4 m3 = (position[2] + position[0]) / 2.0f;
        glm::vec4 color1 = (vertex_color[0] + vertex_color[1]) / 2.0f;
        glm::vec4 color2 = (vertex_color[1] + vertex_color[2]) / 2.0f;
        glm::vec4 color3 = (vertex_color[2] + vertex_color[0]) / 2.0f;
        return Triangle(m1, m2, m3, color1, color2, color3, -1);
    }

    glm::vec3 normal() {
        glm::vec3 s1 = position[1] - position[0];
        glm::vec3 s2 = position[2] - position[0];
        return glm::normalize(glm::cross(s1, s2));
    }

    glm::vec3 centroid() {
        return (position[0] + position[1] + position[2]) / 3.0f;
    }

    bool intersects(glm::vec3 direction, glm::vec3 pos, float &t) {
        glm::vec3 n = normal();

        if (glm::dot(direction, n) == 0) {
            return false;
        }

        t = glm::dot(glm::vec3(position[0]) - pos, n) / glm::dot(direction, n);

        if (t < 0) {
            return false;
        }


        glm::vec3 intersection = pos + direction * t;

        for (int i = 0; i < 3; i++) {
            //First side and second side
            glm::vec3 s1 = position[(i + 1) % 3] - position[i];
            glm::vec3 s2 = position[(i + 2) % 3] - position[i];

            //If it's outside the triangle, then the side made by the intersection - position should be in same dir
            if (glm::dot(glm::cross(s1, s2), glm::cross(intersection - glm::vec3(position[i]), s2)) < 0) {
                return false;
            }
        }

        return true;


    }
};

class kdTree {
public:
    class Node {
    public:
        Node(Node *left, Node *right, kdBox *box,
             std::vector<Triangle> *sceneObject) : left(left), right(right),
                                                   box(box), sceneObject(sceneObject) {};

        Node *left;
        Node *right;
        kdBox *box;
        std::vector<Triangle> *sceneObject;

        bool isLeaf() {
            return sceneObject->size() == 1;
        }

    };

    Node *root;
    int num = 0;
    int scene = 0;
    int count = 0;

    kdTree(std::vector<Triangle> *objects) {
        this->root = new Node(NULL, NULL, NULL, objects);
    };


//    void buildTree(kdBox box);

//    void intersectedObjects(glm::vec3 position, glm::vec3 direction, kdTree::Node *curNode, std::vector<Triangle> &v);

//    kdTree subBuildTree(Node *parent, int depth);

    kdBox *createMergedBoundingBox(std::vector<Triangle> *sortedObjects, int start, int end) {
        kdBox *box = new kdBox();
        for (int i = start; i < end; i++) {
            box->merge((*sortedObjects)[i].getBoundingBox());
        }
        return box;
    }

    std::vector<Triangle> *copyVector(std::vector<Triangle> *v) {
        std::vector<Triangle> *newV = new std::vector<Triangle>;
        for (int i = 0; i < v->size(); i++) {
            newV->push_back((*v)[i]);
        }
        return newV;
    }

    void intersectedObjects(glm::vec3 position, glm::vec3 direction, kdTree::Node *curNode, std::vector<Triangle> &v) {
        count++;
        const double z = 0.0;
        if (curNode->box->intersect(position, direction, const_cast<double &>(z), const_cast<double &>(z))) {
            if (curNode->isLeaf()) {
                v.push_back((*(curNode->sceneObject))[0]);
            } else {
                intersectedObjects(position, direction, curNode->left, v);
                intersectedObjects(position, direction, curNode->right, v);
            }
        }


    }

    void buildTree(kdBox box) {
        root->box = new kdBox(box.getMin(), box.getMax());
        subBuildTree(root, 0);
    }

    void subBuildTree(kdTree::Node *parent, int depth) {

        std::vector<Triangle> *objects = parent->sceneObject;

        if (parent->sceneObject->size() != 1) { //&& depth == 10
            // X DIMENSION

            int size = objects->size();

            std::vector<Triangle> *sortedXObjects = copyVector(objects);
            std::sort(sortedXObjects->begin(),
                      sortedXObjects->end(),
                      [](Triangle lhs, Triangle rhs) {
                          return lhs.getBoundingBox().getMin()[0] < rhs.getBoundingBox().getMin()[0];
                      });
            kdBox *totalXBox = createMergedBoundingBox(sortedXObjects, 0, sortedXObjects->size());
            kdBox *leftXHalf = createMergedBoundingBox(sortedXObjects, 0, sortedXObjects->size() / 2);
            kdBox *rightXHalf = createMergedBoundingBox(sortedXObjects, sortedXObjects->size() / 2,
                                                        sortedXObjects->size());
            double sumOfX = leftXHalf->area() + rightXHalf->area();
            //double sumOfX = (*sortedXObjects)[size - 1].getBoundingBox().getMin()[0] - (*sortedXObjects)[0].getBoundingBox().getMin()[0];

            // Y DIMENSION
            std::vector<Triangle> *sortedYObjects = copyVector(objects);
            std::sort(sortedYObjects->begin(),
                      sortedYObjects->end(),
                      [](Triangle lhs, Triangle rhs) {
                          return lhs.getBoundingBox().getMin()[1] < rhs.getBoundingBox().getMin()[1];
                      });
            kdBox *totalYBox = createMergedBoundingBox(sortedYObjects, 0, sortedYObjects->size());
            kdBox *leftYHalf = createMergedBoundingBox(sortedYObjects, 0, sortedYObjects->size() / 2);
            kdBox *rightYHalf = createMergedBoundingBox(sortedYObjects, sortedYObjects->size() / 2,
                                                        sortedYObjects->size());
            double sumOfY = leftYHalf->area() + rightYHalf->area();
            //double sumOfY = (*sortedYObjects)[size - 1].getBoundingBox().getMin()[1] - (*sortedYObjects)[0].getBoundingBox().getMin()[1];

            // Z DIMENSION
            std::vector<Triangle> *sortedZObjects = copyVector(objects);
            std::sort(sortedZObjects->begin(),
                      sortedZObjects->end(),
                      [](Triangle lhs, Triangle rhs) {
                          return lhs.getBoundingBox().getMin()[2] < rhs.getBoundingBox().getMin()[2];
                      });
            kdBox *totalZBox = createMergedBoundingBox(sortedZObjects, 0, sortedZObjects->size());
            kdBox *leftZHalf = createMergedBoundingBox(sortedZObjects, 0, sortedZObjects->size() / 2);
            kdBox *rightZHalf = createMergedBoundingBox(sortedZObjects, sortedZObjects->size() / 2,
                                                        sortedZObjects->size());
            double sumOfZ = leftZHalf->area() + rightZHalf->area();
            //double sumOfZ = (*sortedZObjects)[size - 1].getBoundingBox().getMin()[2] - (*sortedZObjects)[0].getBoundingBox().getMin()[2];

            // Pick best dimension
            kdBox *bestLeft;
            kdBox *bestRight;
            std::vector<Triangle> *bestList;

            if (sumOfX < sumOfY && sumOfX < sumOfZ) {
                bestLeft = leftXHalf;
                bestRight = rightXHalf;
                bestList = sortedXObjects;
            } else if (sumOfY < sumOfX && sumOfY < sumOfZ) {
                bestLeft = leftYHalf;
                bestRight = rightYHalf;
                bestList = sortedYObjects;
            } else {
                bestLeft = leftZHalf;
                bestRight = rightZHalf;
                bestList = sortedZObjects;
            }

            //for (int i = 0; i < bestLeft.size() / 2)
            std::vector<Triangle> *vLeft = new std::vector<Triangle>;
            for (int i = 0; i < bestList->size() / 2; i++) {
                vLeft->push_back((*bestList)[i]);
            }
            parent->left = new kdTree::Node(NULL, NULL, bestLeft, vLeft);
            subBuildTree(parent->left, depth + 1);

            std::vector<Triangle> *vRight = new std::vector<Triangle>;
            for (int i = bestList->size() / 2; i < bestList->size(); i++) {
                vRight->push_back((*bestList)[i]);
            }
            parent->right = new kdTree::Node(NULL, NULL, bestRight, vRight);
            subBuildTree(parent->right, depth + 1);
        }
    }


};

float make_single_factor(Triangle *tri1, Triangle *tri2) {
//        matrix = new float[triangles.size * triangles.size()];
//    Triangle *tri1 = &triangles[tri1_index];
//    Triangle *tri2 = &triangles[tri2_index];

    glm::vec3 c1 = tri1->centroid();
    glm::vec3 c2 = tri2->centroid();

    glm::vec3 n1 = tri1->normal();
    glm::vec3 n2 = tri2->normal();


    glm::vec3 cDist = c2 - c1;
    float distance = glm::length(cDist);
    cDist = glm::normalize(cDist);
    float angle1 = glm::acos(glm::dot(n1, cDist));
    float angle2 = glm::acos(glm::dot(n2, cDist));

    return glm::abs(glm::cos(angle1) * glm::cos(angle2) / (3.14 * distance * distance));
}

class SceneObject {
public:
    std::vector<glm::vec4> colors;
    glm::vec3 position;
    glm::mat3 rotation;
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<Triangle> triangles;
    kdTree *tree;
    kdBox *box;
    //    float *matrix;

    void makeTriangles(int level) {
        if (level == 0) {
            for (int i = 0; i < faces.size(); i++) {
                triangles.push_back(
                        Triangle(vertices[faces[i].x], vertices[faces[i].y], vertices[faces[i].z], colors[faces[i].x],
                                 colors[faces[i].y], colors[faces[i].z], i));
            }
        } else {
            std::vector<glm::vec4> vertices_copy;
            std::vector<glm::uvec3> faces_copy;
            std::vector<glm::vec4> colors_copy;

            for (int i = 0; i < vertices.size(); i++) {
                vertices_copy.push_back(vertices[i]);
            }
            for (int i = 0; i < faces.size(); i++) {
                faces_copy.push_back(faces[i]);
            }
            for (int i = 0; i < colors.size(); i++) {
                colors_copy.push_back(colors[i]);
            }
            vertices.clear();
            faces.clear();
            colors.clear();
            for (int i = 0; i < faces_copy.size(); i++) {
//            glm::vec4 color = glm::vec4()
                Triangle tri(vertices_copy[faces_copy[i].x], vertices_copy[faces_copy[i].y],
                             vertices_copy[faces_copy[i].z], colors_copy[faces_copy[i].x], colors_copy[faces_copy[i].y],
                             colors_copy[faces_copy[i].z], -1);
                recursiveTris(level, tri);
            }
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
            triangles.push_back(Triangle(tri.position[0], mid.position[0], mid.position[2], mid.vertex_color[0],
                                         mid.vertex_color[1], mid.vertex_color[2], tri_size));
            vertices.push_back(tri.position[1]);
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[1]);
            triangles.push_back(Triangle(tri.position[1], mid.position[0], mid.position[1], mid.vertex_color[0],
                                         mid.vertex_color[1], mid.vertex_color[2], tri_size + 1));
            vertices.push_back(tri.position[2]);
            vertices.push_back(mid.position[1]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(tri.position[2], mid.position[1], mid.position[2], mid.vertex_color[0],
                                         mid.vertex_color[1], mid.vertex_color[2], tri_size + 2));
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[1]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(mid.position[0], mid.position[1], mid.position[2], mid.vertex_color[0],
                                         mid.vertex_color[1], mid.vertex_color[2], tri_size + 3));

//            for (int i = 0; i < 4; i++) {
//                triangles[triangles.size() - 4 + i].diffuse_constant = glm::vec4(1.0, 1.0, 1.0, 1.0);
//            }
            for (int i = 0; i < 12; i++) {
//                if (i % 2 == 0)
                colors.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));
//                else
//                    diffuse.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
            }
            faces.push_back(glm::vec3(size, size + 1, size + 2));
            faces.push_back(glm::vec3(size + 3, size + 4, size + 5));
            faces.push_back(glm::vec3(size + 6, size + 7, size + 8));
            faces.push_back(glm::vec3(size + 9, size + 10, size + 11));
            return;
        } else {
            Triangle tri1(tri.position[0], mid.position[0], mid.position[2], mid.vertex_color[0], mid.vertex_color[1],
                          mid.vertex_color[2], -1);
            Triangle tri2(tri.position[1], mid.position[0], mid.position[1], mid.vertex_color[0], mid.vertex_color[1],
                          mid.vertex_color[2], -1);
            Triangle tri3(tri.position[2], mid.position[1], mid.position[2], mid.vertex_color[0], mid.vertex_color[1],
                          mid.vertex_color[2], -1);
            Triangle tri4(mid.position[0], mid.position[1], mid.position[2], mid.vertex_color[0], mid.vertex_color[1],
                          mid.vertex_color[2], -1);

            recursiveTris(level - 1, tri1);
            recursiveTris(level - 1, tri2);
            recursiveTris(level - 1, tri3);
            recursiveTris(level - 1, tri4);
        }
    }

    void updateColors(int level) {
        if (level == 0) {
            for (int i = 0; i < triangles.size(); i++) {
                colors[faces[i].x] = triangles[i].color;
                colors[faces[i].y] = triangles[i].color;
                colors[faces[i].z] = triangles[i].color;
            }

            return;
        }


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

    void pre_process(int numThreads) {


        int triangles_per_thread = triangles.size() / numThreads;

        if (triangles.size() % numThreads != 0) {
            numThreads++;
        }




        std::thread *drawThreads[numThreads];
        for (int i = 0, k = 0; i < triangles.size(), k < numThreads; i += triangles_per_thread, k++) {
            int max = i + triangles_per_thread < triangles.size() ? i + triangles_per_thread : triangles.size();
            std::vector<Triangle> *t = &triangles;
            kdTree *treeCopy = tree;
//            auto *func = make_single_factor;
            drawThreads[k] = new std::thread([treeCopy, t, max, i] {
                std::vector<Triangle> triangles = *t;
//                std::cout << triangles.size() << std::endl;
//                std::cout << "started thread" << std::endl;
                for (int tri1_index = i; tri1_index < max; tri1_index++) {
                    auto tri1 = (*t)[tri1_index];
                    for (int tri2_index = 0; tri2_index < triangles.size(); tri2_index += 1) {
                        Triangle tri2 = (*t)[tri2_index];
                        float factor = make_single_factor(&tri1, &tri2);
                        if (factor < .00001) {
                            continue;
                        }
                        if (tri1_index == tri2_index) {
                            continue;
                        }

                        glm::vec3 direction = glm::normalize(tri2.centroid() - tri1.centroid());
                        bool intersected = false;
                        float earliestT = glm::length(tri2.centroid() - tri1.centroid());
                        std::vector<Triangle> possible_list;
                        treeCopy->intersectedObjects(tri1.centroid(), direction, treeCopy->root, possible_list);
//                        possible_list = triangles;
                        for (int j = 0; j < possible_list.size(); j++) {
                            if (possible_list[j].index == tri2_index || possible_list[j].index == tri1_index) {
                                continue;
                            }

                            float possibleT;
                            if (possible_list[j].intersects(direction, tri1.centroid(), possibleT) &&
                                possibleT < earliestT) {
                                intersected = true;
                                break;
                            }
                        }

                        if (!intersected) {
                            (*t)[tri1_index].form_factors[tri2_index] = factor;
                        }


                    }
                }
            });
        }

        for (int l = 0; l < numThreads; l++) {
            drawThreads[l]->join();
        }

        //tree->count = 0;


    }

    float attenuation(float distance) {
        float c = 1.5f;
        return 1 / (c + c * distance + c * distance * distance);
//        return 1.0f;
    }

    void calculate_light(LightSource &lightSource, int numThreads, bool direct) {
        //Leftover light is initially 0
        //Leftover accumulates as it gets light
        //Diffuse is the amount of light to diffuse currently
        //Vertices will contain the accumulated light
        if (!lightSource.used) {
            int triangles_per_thread = triangles.size() / numThreads;

            if (triangles.size() % numThreads != 0) {
                numThreads++;
            }


            std::thread *drawThreads[numThreads];
            for (int i = 0, k = 0; i < triangles.size(), k < numThreads; i += triangles_per_thread, k++) {
                int max = i + triangles_per_thread < triangles.size() ? i + triangles_per_thread : triangles.size();
                std::vector<Triangle> *t = &triangles;
                kdTree *treeCopy = tree;
                drawThreads[k] = new std::thread([treeCopy, t, max, i, &lightSource, direct] {
                    for (int i = 0; i < (*t).size(); i++) {
                        //For each triangle
                        Triangle *tri = &((*t)[i]);
                        Triangle md = tri->mdptTri();
                        glm::vec3 normal = tri->normal();

                        //Calculates centroid of the 4 sub-triangles
                        glm::vec4 samples[4];
                        samples[0] = glm::vec4(tri->centroid(), 1.0);
                        samples[1] = (tri->position[0] + md.position[0] + md.position[2]) / 3.0;
                        samples[2] = (tri->position[1] + md.position[0] + md.position[1]) / 3.0;
                        samples[3] = (tri->position[2] + md.position[1] + md.position[2]) / 3.0;

                        float visibilityFactor = 0.0;
                        //For each sub-centroid, calculates light direction and sees if it intersects any triangles
                        for (int k = 0; k < 4; k++) {
                            glm::vec3 lightDir = glm::normalize(samples[k] - lightSource.position);

                            std::vector<Triangle> possible_triangles;
                            treeCopy->intersectedObjects(lightSource.position, lightDir, treeCopy->root, possible_triangles);
                            bool intersected = false;
                            //Multiplying this with lightDir + lightSource gets the centroid
                            float earliestT = glm::length(samples[k] - lightSource.position);
                            for (int j = 0; j < possible_triangles.size(); j++) {
                                //If there is a single triangle blocking the light, then return true
                                if (i == possible_triangles[j].index) {
                                    continue;
                                }

                                float possibleT;
                                if (possible_triangles[j].intersects(lightDir, lightSource.position, possibleT) &&
                                    earliestT > possibleT) {
                                    //The ray must intersect the triangle and must do so earlier than the triangle we are computing light for
                                    intersected = true;
                                    break;
                                }

                            }

                            if (!intersected) {
                                visibilityFactor += 0.25;
                            }

                            /*
                            std::cout << "Triangles size: " << possible_triangles.size() << std::endl;
                            std::cout << tree->count << std::endl;
                            tree->count = 0;
                            */
                        }


                        glm::vec4 centroid = glm::vec4(tri->centroid(), 1.0);
                        glm::vec3 lightDir = glm::normalize(centroid - lightSource.position);
                        float distance = 0.0;

                        if (direct) {
                            float distance = glm::distance(lightSource.position, centroid);
                        } else {
                            float distance = glm::distance(lightSource.position, centroid);
                            //                        std::cout << "not direct" << std::endl;
                        }
                        float c = 1.5f;
                        float atten = 1 / (c + c * distance + c * distance * distance);
                        if (direct) {
                            tri->diffuse_lighting =
                                    tri->diffuse_constant * atten * lightSource.intensity *
                                    glm::abs(glm::dot(lightDir, normal)) * visibilityFactor;
                        } else {
                            float area = 105 * 130 / 4;
                            tri->diffuse_lighting =
                                    tri->diffuse_constant * atten * lightSource.intensity *
                                    glm::abs(glm::dot(lightDir, normal)) * area * visibilityFactor;
                        }


                        tri->color = tri->diffuse_lighting;

                        //                tri->color = glm::vec4(1.0f,1.0f, 1.0f, 1.0f);
                    }
                });
            }
            for (int l = 0; l < numThreads; l++) {
                drawThreads[l]->join();
            }

            lightSource.used = true;
        } else {

            int triangles_per_thread = triangles.size() / numThreads;

            if (triangles.size() % numThreads != 0) {
                numThreads++;
            }


            std::thread *drawThreads[numThreads];
            for (int i = 0, k = 0; i < triangles.size(), k < numThreads; i += triangles_per_thread, k++) {
                int max = i + triangles_per_thread < triangles.size() ? i + triangles_per_thread : triangles.size();
                std::vector<Triangle> *t = &triangles;
                kdTree *treeCopy = tree;
                drawThreads[k] = new std::thread([treeCopy, t, max, i] {
                                                     for (int tri_index = i; tri_index < max; tri_index++) {
                                                         Triangle *tri1 = &((*t)[tri_index]);
                                                         for (std::map<int, float>::iterator it = tri1->form_factors.begin();
                                                              it != tri1->form_factors.end(); it++) {
                                                             if (it->first == tri_index) {
                                                                 continue;
                                                             }

                                                             Triangle tri2 = ((*t)[it->first]);
                                                             float form_factor = tri1->form_factors[it->first];

                                                             tri1->accum += (form_factor * tri2.diffuse_lighting) * tri2.getArea();
                                                         }
                                                     }
                                                 }
                );
            }
            for (int l = 0; l < numThreads; l++) {
                drawThreads[l]->join();
            }
            for (int i = 0; i < triangles.size(); i++) {
                Triangle tri = triangles[i];
                triangles[i].diffuse_lighting = triangles[i].diffuse_constant * triangles[i].accum;
                if (triangles[i].diffuse_lighting.x > 1) {
                    triangles[i].color.x = 1;
                    triangles[i].diffuse_lighting.x = 1;
                } else {
                    triangles[i].color.x += triangles[i].diffuse_lighting.x;
                }
                if (triangles[i].diffuse_lighting.y > 1) {
                    triangles[i].color.y = 1;
                    triangles[i].diffuse_lighting.y = 1;
                } else {
                    triangles[i].color.y += triangles[i].diffuse_lighting.y;
                }
                if (triangles[i].diffuse_lighting.z > 1) {
                    triangles[i].color.z = 1;
                    triangles[i].diffuse_lighting.z = 1;
                } else {
                    triangles[i].color.z += triangles[i].diffuse_lighting.z;
                }
                if (triangles[i].diffuse_lighting.w > 1) {
                    triangles[i].color.w = 1;
                    triangles[i].diffuse_lighting.w = 1;
                } else {
                    triangles[i].color.w += triangles[i].diffuse_lighting.w;
                }
//                triangles[i].color += triangles[i].diffuse_lighting;
                triangles[i].accum = glm::vec4(0.0, 0.0, 0.0, 0.0);
            }
        }
    }

    void loadScene(objl::Loader loader) {
        // THIS IS FOR PER OBJECT LOADING
//        int total_size = 0;
//        int face_size = 0;
//        for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
//            auto mesh = loader.LoadedMeshes[i];
//            SceneObject object;
//            total_size += mesh.Vertices.size();
//            face_size += mesh.Indices.size();
//            for (int j = 0; j < mesh.Vertices.size(); j++) {
//                auto vert = mesh.Vertices[j];
//                object.vertices.push_back(glm::vec4(vert.Position.X, vert.Position.Y, vert.Position.Z, 1));
//            }
//            for (int j = 0; j < mesh.Indices.size(); j+=3) {
//                auto index1 = mesh.Indices[j];
//                auto index2 = mesh.Indices[j];
//                auto index3 = mesh.Indices[j];
//                object.faces.push_back(glm::vec3(index1, index2, index3));
//                Triangle tri(object.vertices[index1], object.vertices[index2], object.vertices[index3]);
//            }
//        }
        for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
            auto mesh = loader.LoadedMeshes[i];
            for (int j = 0; j < mesh.Vertices.size(); j++) {
                colors.push_back(glm::vec4(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z, 1));
            }
        }
        for (int i = 0; i < loader.LoadedVertices.size(); i++) {
            auto vert = loader.LoadedVertices[i];
            vertices.push_back(glm::vec4(vert.Position.X, vert.Position.Y, vert.Position.Z, 1));
        }
        for (int i = 0; i < loader.LoadedIndices.size(); i += 3) {
            auto index1 = loader.LoadedIndices[i];
            auto index2 = loader.LoadedIndices[i + 1];
            auto index3 = loader.LoadedIndices[i + 2];
            faces.push_back(glm::vec3(index1, index2, index3));
            Triangle tri(vertices[index1], vertices[index2], vertices[index3], colors[index1], colors[index2],
                         colors[index3], faces.size() - 1);
        }


        //
    }

};

class RadiosityScene {
    std::vector<SceneObject> objects;


};

// Note: you can put kd-tree here

#endif //GLSL_RADIOSITYSCENE_H
