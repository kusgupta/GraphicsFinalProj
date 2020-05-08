//
// Created by Kushan Gupta on 4/29/20.
//

#ifndef GLSL_RADIOSITYSCENE_H
#define GLSL_RADIOSITYSCENE_H
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>
#include <thread>
#include "OBJ_Loader.h"


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
    glm::vec4 vertex_color[3];
    glm::vec4 diffuse_constant = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    glm::vec4
    glm::vec4 diffuse_lighting;
    glm::vec4 accum = glm::vec4(0.0, 0.0, 0.0, 0.0);
    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 0.0);
    float *form_factors;

    Triangle(glm::vec4 pos1, glm::vec4 pos2, glm::vec4 pos3, glm::vec4 color1, glm::vec4 color2, glm::vec4 color3) {
        position[0] = pos1;
        position[1] = pos2;
        position[2] = pos3;
        vertex_color[0] = color1;
        vertex_color[1] = color2;
        vertex_color[2] = color3;
        diffuse_constant = (color1 + color2 + color3) / 3.0f;
    }

    Triangle mdptTri() {
        glm::vec4 m1 = (position[0] + position[1]) / 2.0f;
        glm::vec4 m2 = (position[1] + position[2]) / 2.0f;
        glm::vec4 m3 = (position[2] + position[0]) / 2.0f;
        glm::vec4 color1 = (vertex_color[0] + vertex_color[1]) / 2.0f;
        glm::vec4 color2 = (vertex_color[1] + vertex_color[2]) / 2.0f;
        glm::vec4 color3 = (vertex_color[2] + vertex_color[0]) / 2.0f;
        return Triangle(m1, m2, m3, color1, color2, color3);
    }

    glm::vec3 normal() {
        glm::vec3 s1 = position[1] - position[0];
        glm::vec3 s2 = position[2] - position[0];
        return glm::normalize(glm::cross(s1, s2));
    }

    glm::vec3 centroid() {
        return (position[0] + position[1] + position[2]) / 3.0f;
    }

    bool intersects(glm::vec3 direction, glm::vec3 pos, float& t) {
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

        /*
        glm::vec3 a_coords = position[0];
        glm::vec3 b_coords = position[1];
        glm::vec3 c_coords = position[2];



        glm::vec3 vab = (b_coords - a_coords);
        glm::vec3 vbc = (c_coords - b_coords);
        glm::vec3 vca = (a_coords - c_coords);

        glm::vec3 n = glm::cross(vab, -vca);
        double denominator = glm::dot(direction, n);
        double t = glm::dot(a_coords - glm::vec3(pos), n)/denominator;
        if( t <= .0001 ) {
            return false;
        }

        glm::vec3 p = pos + t * direction;
        glm::vec3 pa = (p - a_coords);
        glm::vec3 pb = (p - b_coords);
        glm::vec3 pc = (p - c_coords);


        bool doesIntersect = glm::dot(glm::cross(vab, pa),n) >= 0;
        doesIntersect = doesIntersect && (glm::dot(glm::cross(vbc, pb),n) >= 0);
        doesIntersect = doesIntersect && (glm::dot(glm::cross(vca, pc),n) >= 0);
        return doesIntersect;
        */
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
    float *matrix;

    void makeTriangles(int level) {
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
                         vertices_copy[faces_copy[i].z], colors_copy[faces_copy[i].x], colors_copy[faces_copy[i].y], colors_copy[faces_copy[i].z]);
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
            triangles.push_back(Triangle(tri.position[0], mid.position[0], mid.position[2], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]));
            vertices.push_back(tri.position[1]);
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[1]);
            triangles.push_back(Triangle(tri.position[1], mid.position[0], mid.position[1], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]));
            vertices.push_back(tri.position[2]);
            vertices.push_back(mid.position[1]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(tri.position[2], mid.position[1], mid.position[2], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]));
            vertices.push_back(mid.position[0]);
            vertices.push_back(mid.position[1]);
            vertices.push_back(mid.position[2]);
            triangles.push_back(Triangle(mid.position[0], mid.position[1], mid.position[2], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]));

//            for (int i = 0; i < 4; i++) {
//                triangles[triangles.size() - 4 + i].diffuse_constant = glm::vec4(1.0, 1.0, 1.0, 1.0);
//            }
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
            Triangle tri1(tri.position[0], mid.position[0], mid.position[2], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]);
            Triangle tri2(tri.position[1], mid.position[0], mid.position[1], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]);
            Triangle tri3(tri.position[2], mid.position[1], mid.position[2], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]);
            Triangle tri4(mid.position[0], mid.position[1], mid.position[2], mid.vertex_color[0], mid.vertex_color[1], mid.vertex_color[2]);

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

    void make_form_factors_threads(int numThreads) {
//        matrix = new float[triangles.size * triangles.size()];
        int triangles_per_thread = triangles.size() / numThreads;

        if (triangles.size() % numThreads != 0) {
            numThreads++;
        }


        std::thread *drawThreads[numThreads];
        for (int i = 0, k = 0; i < triangles.size(), k < numThreads; i+=triangles_per_thread, k++) {
            int max = i + triangles_per_thread < triangles.size() ? i + triangles_per_thread : triangles.size();
            drawThreads[k] = new std::thread([&] {

                for (int l = i; l < max; l++) {
                    Triangle *tri1 = &triangles[l];
                    tri1->form_factors = new float[triangles.size()];
                    for (int j = 0; j < triangles.size(); j++) {
                        if (j == i) {
                            continue;
                        }

                        Triangle *tri2 = &triangles[j];

                        glm::vec3 c1 = tri1->centroid();
                        glm::vec3 c2 = tri2->centroid();

                        glm::vec3 n1 = tri1->normal();
                        glm::vec3 n2 = tri2->normal();


                        glm::vec3 cDist = c2 - c1;
                        float distance = glm::length(cDist) / 10;
                        cDist = glm::normalize(cDist);
                        float angle1 = glm::acos(glm::dot(n1, cDist));
                        float angle2 = glm::acos(glm::dot(n2, cDist));


                        float form_factor = glm::abs(glm::cos(angle1) * glm::cos(angle2) / (3.14 * distance * distance));
                        /*
                        if (form_factor >= 0.01) {
                            std::cout << "lol" << std::endl;
                        }
                        */

                        tri1->form_factors[j] = form_factor;
                    }
                }
            });
        }
        for (int i = 0; i < numThreads; i++) {
            drawThreads[i]->join();
        }
    }

    void make_form_factors() {
//        matrix = new float[triangles.size * triangles.size()];
        for (int i = 0; i < triangles.size(); i++) {
            Triangle *tri1 = &triangles[i];
            tri1->form_factors = new float[triangles.size()];
            for (int j = 0; j < triangles.size(); j++) {
                if (j == i) {
                    continue;
                }

                Triangle *tri2 = &triangles[j];

                glm::vec3 c1 = tri1->centroid();
                glm::vec3 c2 = tri2->centroid();

                glm::vec3 n1 = tri1->normal();
                glm::vec3 n2 = tri2->normal();


                glm::vec3 cDist = c2 - c1;
                float distance = glm::length(cDist) / 10;
                cDist = glm::normalize(cDist);
                float angle1 = glm::acos(glm::dot(n1, cDist));
                float angle2 = glm::acos(glm::dot(n2, cDist));



                float form_factor = glm::abs(glm::cos(angle1) * glm::cos(angle2) / (3.14 * distance * distance));
                /*
                if (form_factor >= 0.01) {
                    std::cout << "lol" << std::endl;
                }
                */

                tri1->form_factors[j] = form_factor;
            }

        }
    }

    float attenuation(float distance) {
        float c = .10f;
        return 1 / (c + c * distance);// + c * distance * distance);
//        return 1.0f;
    }

    std::vector<Triangle> unobstructed_triangles(int tri1_index) {
//        return triangles;
        std::vector<Triangle> unobstructed;
        std::vector<Triangle> rand_list;

        auto tri1 = triangles[tri1_index];
        //TODO: DON'T USE RAND
//        for (int i = 0; i < 100; i++) {
//            int index = rand() % triangles.size();
//            rand_list.push_back(triangles[index]);
//        }

        for (int i = 0; i < triangles.size(); i++) {
            Triangle tri2 = triangles[i];
            if (i == tri1_index) {
                continue;
            }

            glm::vec3 direction = glm::normalize(tri2.centroid() - tri1.centroid());
            bool intersected = false;
            float earliestT = glm::length(tri2.centroid() - tri1.centroid());
            for (int j = 0; j < triangles.size(); j++) {
                if (j == i || j == tri1_index) {
                    continue;
                }

                float possibleT;
                if (triangles[j].intersects(direction, tri1.centroid(), possibleT) && possibleT < earliestT){
                    intersected = true;
                    break;
                }
            }

            if (!intersected) {
                unobstructed.push_back(tri2);
            }
        }
        return unobstructed;
    }

    void calculate_light(LightSource &lightSource) {
        //Leftover light is initially 0
        //Leftover accumulates as it gets light
        //Diffuse is the amount of light to diffuse currently
        //Vertices will contain the accumulated light
        if (!lightSource.used) {
            for (int i = 0; i < triangles.size(); i++) {
                //For each triangle
                Triangle *tri = &triangles[i];
                glm::vec3 normal = tri->normal();

                //Calculates centroid and light direction
                glm::vec4 centroid(0.0, 0.0, 0.0, 0.0);

                for (int i = 0; i < 3; i++) {
                    centroid += tri->position[i];
                }

                centroid /= 3.0;


                glm::vec3 lightDir = glm::normalize(centroid - lightSource.position);

                bool intersected = false;
                //Multiplying this with lightDir + lightSource gets the centroid
                float earliestT = glm::length(centroid - lightSource.position);
                for (int j = 0; j < triangles.size(); j++) {
                    //If there is a single triangle blocking the light, then return true
                    if (i == j) {
                        continue;
                    }

                    float possibleT;
                    if (triangles[j].intersects(lightDir, lightSource.position, possibleT) && earliestT > possibleT) {
                        //The ray must intersect the triangle and must do so earlier than the triangle we are computing light for
                        intersected = true;
                        break;
                    }


                }

                if (intersected) {
                    tri->diffuse_lighting = glm::vec4(0.0, 0.0, 0.0, 1);
                }

                else {
                    float distance = glm::distance(lightSource.position, centroid) / 40;
                    tri->diffuse_lighting = tri->diffuse_constant * attenuation(distance) * lightSource.intensity *
                                            glm::abs(glm::dot(lightDir, normal));
                }

                tri->color = tri->diffuse_lighting;

//                tri->color = glm::vec4(1.0f,1.0f, 1.0f, 1.0f);
            }

            lightSource.used = true;
        } else {
            for (int i = 0; i < triangles.size(); i++) {
                Triangle *tri1 = &triangles[i];
                // TODO: Create unobstructed triangle list for the next for loop
//                std::vector<Triangle> unobstructed = triangles;
                std::vector<Triangle> unobstructed = unobstructed_triangles(i);


                for (int j = 0; j < unobstructed.size(); j++) {
                    if (j == i) {
                        continue;
                    }
                    Triangle tri2 = unobstructed[j];
                    if (tri1->form_factors[j] == 0.f) {
                        glm::vec3 c1 = tri1->centroid();
                        glm::vec3 c2 = tri2.centroid();

                        glm::vec3 n1 = tri1->normal();
                        glm::vec3 n2 = tri2.normal();


                        glm::vec3 cDist = c2 - c1;
                        float distance = glm::length(cDist) / 10;
                        cDist = glm::normalize(cDist);
                        float angle1 = glm::acos(glm::dot(n1, cDist));
                        float angle2 = glm::acos(glm::dot(n2, cDist));



                        float form_factor = glm::abs(glm::cos(angle1) * glm::cos(angle2) / (3.14 * distance * distance));
                        /*
                        if (form_factor >= 0.01) {
                            std::cout << "lol" << std::endl;
                        }
                        */

                        tri1->form_factors[j] = form_factor;
                    }
                    float form_factor = tri1->form_factors[j];
                    tri1->accum += (form_factor * tri2.diffuse_lighting);
                }

            }

            for (int i = 0; i < triangles.size(); i++) {
                Triangle tri = triangles[i];
                triangles[i].diffuse_lighting = triangles[i].diffuse_constant * triangles[i].accum;
                triangles[i].color += triangles[i].diffuse_lighting;
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
            for (int j = 0; j < mesh.Vertices.size(); i++) {
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
            Triangle tri(vertices[index1], vertices[index2], vertices[index3], colors[index1], colors[index2], colors[index3]);
        }


        //
    }

};

class RadiosityScene {
    std::vector<SceneObject> objects;


};

#endif //GLSL_RADIOSITYSCENE_H
