#include "bone_geometry.h"
#include "texture_to_render.h"
#include <fstream>
#include <iostream>
#include <glm/gtx/io.hpp>
#include <unordered_map>

/*
 * We put these functions to a separated file because the following jumbo
 * header consists of 20K LOC, and it is very slow to compile.
 */
#include "json.hpp"

using json = nlohmann::json;
namespace {
    const glm::fquat identity(1.0, 0.0, 0.0, 0.0);
}

void Mesh::saveAnimationTo(const std::string& fn)
{
    if (keyFrames.size() < 1)
        return;
    std::ofstream MyFile(fn);
    MyFile << keyFrames.size();
    MyFile << "\n";
    MyFile << keyFrames[0].rel_rot.size();
    MyFile << "\n";
    for (int key_frame = 0; key_frame < keyFrames.size(); key_frame++) {
        for (int joint = 0; joint < keyFrames[0].rel_rot.size(); joint++) {
            glm::fquat j = keyFrames[key_frame].rel_rot[joint];
            MyFile <<  j.w;
            MyFile << " ";
            MyFile << j.x;
            MyFile << " ";
            MyFile << j.y;
            MyFile << " ";
            MyFile << j.z;
            MyFile << "\n";
        }
    }
}

void Mesh::loadAnimationFrom(const std::string& fn)
{
    // FIXME: Load keyframes from json file.
    std::ifstream MyFile(fn);
    int num_joints, num_frames;
    MyFile >> num_frames >> num_joints;
    for (int i = 0; i < num_frames; i++) {
        KeyFrame k;
        for (int j = 0; j < num_joints; j++) {
            float w, x, y, z;
            MyFile >> w >> x >> y >> z;
            glm::fquat q(w, x, y, z);
            k.rel_rot.push_back(q);
        }
        k.rendered = false;
        keyFrames.push_back(k);
    }
}
