#include "config.h"
#include "bone_geometry.h"
#include "texture_to_render.h"
#include <fstream>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <glm/gtx/io.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stdlib.h>
#include "Cylinder.h"
/*
 * For debugging purpose.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	size_t count = std::min(v.size(), static_cast<size_t>(10));
	for (size_t i = 0; i < count; ++i) os << i << " " << v[i] << "\n";
	os << "size = " << v.size() << "\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& bounds)
{
	os << "min = " << bounds.min << " max = " << bounds.max;
	return os;
}



const glm::vec3* Skeleton::collectJointTrans() const
{
	return cache.trans.data();
}

const glm::fquat* Skeleton::collectJointRot() const
{
	return cache.rot.data();
}

// FIXME: Implement bone animation.

void Skeleton::refreshCache(Configuration* target)
{
	if (target == nullptr)
		target = &cache;
	target->rot.resize(joints.size());
	target->trans.resize(joints.size());
	target->angles.resize(joints.size());
	target->axes.resize(joints.size());
	for (size_t i = 0; i < joints.size(); i++) {
		target->angles[i] = glm::angle(joints[i]->orientation);
		target->axes[i] = glm::axis(joints[i]->orientation);
		target->rot[i] = joints[i]->orientation;
		target->trans[i] = joints[i]->position;
	}
}


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void createBones(Joint *currentJoint, std::vector<Bone*> &bones) {
    for (int i = 0; i < currentJoint->children.size(); i++) {
        bones.push_back(new Bone(currentJoint, currentJoint->children[i]));
        createBones(currentJoint->children[i], bones);
    }
}

void Mesh::loadPmd(const std::string& fn)
{
	MMDReader mr;
	mr.open(fn);
	mr.getMesh(vertices, faces, vertex_normals, uv_coordinates);
	computeBounds();
	mr.getMaterial(materials);

	// FIXME: load skeleton and blend weights from PMD file,
	//        initialize std::vectors for the vertex attributes,
	//        also initialize the skeleton as needed
	bool keep_running = true;
	for (int i = 0; keep_running; i++) {
		glm::vec3 world_coord;
		int parent;
		keep_running = mr.getJoint(i, world_coord, parent);
		if (keep_running) {
			skeleton.joints.push_back(new Joint(i, world_coord, parent));
		}
	}
	keep_running = true;
	for (int i = 0; keep_running; i++) {
		glm::vec3 world_coord;
		int parent;
		keep_running = mr.getJoint(i, world_coord, parent);
		if (keep_running) {
			if (parent != -1) {
				skeleton.joints[parent]->children.push_back(skeleton.joints[i]);
				skeleton.joints[parent]->children[skeleton.joints[parent]->children.size() - 1]->T_i = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
			}
			else {
				skeleton.joints[i]->T_i = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
				
			}
		}
	}

	for (int i = 0; i < skeleton.joints.size(); i++) {
		glm::vec3 world_coord;
		Joint *current_joint = skeleton.joints[i];
        if (current_joint->parent_index != -1) {

//            Joint *root = skeleton.joints[i];

                glm::vec3 world_coord_parent = skeleton.joints[current_joint->parent_index]->position;
                current_joint->B_ji = glm::mat4({{1,                                0, 0, 0},
                                                 {0,                                1, 0, 0},
                                                 {0,                                0, 1, 0},
                                                 {world_coord - world_coord_parent, 1}});

        }
        else {
            current_joint->B_ji = glm::mat4({{1,                                0, 0, 0},
                                             {0,                                1, 0, 0},
                                             {0,                                0, 1, 0},
                                             {current_joint->position, 1}});
        }
	}

	for (int i = 0; i < skeleton.joints.size(); i++) {
		Joint* current_joint = skeleton.joints[i];
		if (current_joint->parent_index == - 1) {//Recursively calculate u_i from the root
			current_joint->calculateU_i();
		}
	}

	std::vector<Bone*> bones;
	for (int i = 0; i < skeleton.joints.size(); i++) {
        Joint* current_joint = skeleton.joints[i];
        if (current_joint->parent_index == - 1) {//Recursively calculate u_i from the root
            createBones(current_joint, bones);
        }
	}
	skeleton.bones = bones;

    std::vector<SparseTuple> sparseTuple;
    mr.getJointWeights(sparseTuple);

    for (int i = 0; i < sparseTuple.size(); i++) {
        joint0.push_back(sparseTuple[i].jid0);
        if (sparseTuple[i].jid1 != -1) {
            joint1.push_back(sparseTuple[i].jid1);
            vector_from_joint1.push_back(vertices[sparseTuple[i].vid] - glm::vec4(skeleton.joints[sparseTuple[i].jid1]->position, 1));
        }

        else {
            joint1.push_back(-1);
            vector_from_joint1.push_back(glm::vec3(0.0, 0.0, 0.0));
        }

        weight_for_joint0.push_back(sparseTuple[i].weight0);
        vector_from_joint0.push_back(vertices[sparseTuple[i].vid] - glm::vec4(skeleton.joints[sparseTuple[i].jid0]->position, 1));
    }

    skeleton.refreshCache(const_cast<Configuration *>(getCurrentQ()));

}

int Mesh::getNumberOfBones() const
{
	return skeleton.joints.size();
}

void Mesh::computeBounds()
{
	bounds.min = glm::vec3(std::numeric_limits<float>::max());
	bounds.max = glm::vec3(-std::numeric_limits<float>::max());
	for (const auto& vert : vertices) {
		bounds.min = glm::min(glm::vec3(vert), bounds.min);
		bounds.max = glm::max(glm::vec3(vert), bounds.max);
	}
}

void Mesh::updateAnimation(float t)
{
    totalRunTime += t;
    if (totalRunTime >= keyFrames.size() - 1) {
        totalRunTime = keyFrames.size() - 1;
    }

    int currentFrame = int(totalRunTime);
    int nextFrame = currentFrame + 1;
    KeyFrame target;

    if (nextFrame == keyFrames.size()){
        nextFrame--;
    }

    KeyFrame::interpolate(keyFrames[currentFrame], keyFrames[nextFrame], totalRunTime - float(int(totalRunTime)), target);
    for (int i = 0; i < skeleton.joints.size(); i++) {
    //Re-initializes skeleton
        Joint *current_joint = skeleton.joints[i];
        current_joint->T_i = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        current_joint->D_i = current_joint->U_i;
    }
//    printf("%f\n", totalRunTime);
    for (int j = 0; j < skeleton.joints.size(); j++) {
//        skeleton.joints[j]->orientation = k.rel_rot[j];
        Joint *current_joint = skeleton.joints[j];
        current_joint->orientation = target.rel_rot[j];
        current_joint->rel_orientation = target.rel_rot[j];

        if (current_joint->parent_index != -1) {
            Joint *parent = skeleton.joints[current_joint->parent_index];
            skeleton.joints[j]->applyTransformations(current_joint, parent, glm::toMat4(current_joint->orientation));
            current_joint->transformRecursive(parent);
        }
        else {
            skeleton.joints[j]->applyTransformations(current_joint, nullptr, glm::toMat4(current_joint->orientation));
            current_joint->transformRecursive(current_joint);
        }
    }
	skeleton.refreshCache(&currentQ_);
	// FIXME: Support Animation Here
}

const Configuration*
Mesh::getCurrentQ() const
{
	return &currentQ_;
}

void KeyFrame::interpolate(const KeyFrame &from, const KeyFrame &to, float tau, KeyFrame &target) {
    for (int i = 0; i < from.rel_rot.size(); i++) {
        glm::fquat q1 = from.rel_rot[i];
        glm::fquat q2 = to.rel_rot[i];
        target.rel_rot.push_back(glm::slerp(q1, q2, tau));
    }
}
