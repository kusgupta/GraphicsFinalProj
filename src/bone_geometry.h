#ifndef BONE_GEOMETRY_H
#define BONE_GEOMETRY_H

#include <GL/glew.h>
#include <ostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mmdadapter.h>
#include <unordered_map>
#include <glm/ext.hpp>
#include <iostream>
#include "texture_to_render.h"

class TextureToRender;

struct BoundingBox {
	BoundingBox()
		: min(glm::vec3(-std::numeric_limits<float>::max())),
		max(glm::vec3(std::numeric_limits<float>::max())) {}
	glm::vec3 min;
	glm::vec3 max;
};



struct Joint {
	Joint()
		: joint_index(-1),
		  parent_index(-1),
		  position(glm::vec3(0.0f)),
		  init_position(glm::vec3(0.0f))
	{
	}
	Joint(int id, glm::vec3 wcoord, int parent)
		: joint_index(id),
		  parent_index(parent),
		  position(wcoord),
		  init_position(wcoord),
		  init_rel_position(init_position)
	{
	}

	void applyTransformations(Joint *curBone, Joint *parent, glm::mat4 newT_i){
		T_i = newT_i * T_i;
		// b = initial position times t matrix on each change.
//		B_ji = glm::mat4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {glm::vec4({init_rel_position, 1}) * T_i});
		if (curBone->parent_index != -1) {
			D_i = parent->D_i * B_ji * T_i;
		}
		else {
			D_i = B_ji * T_i;
		}
		orientation = glm::quat_cast(T_i);
        rel_orientation = orientation;
    }

	void transformRecursive(Joint *parent) {
		for (int i = 0; i < parent->children.size(); i++) {
			Joint *child = parent->children[i];
//            B_ji = glm::mat4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {glm::vec4({init_rel_position, 1}) * T_i});
            child->D_i = parent->D_i * child->B_ji * child->T_i;
			child->position = child->D_i * glm::vec4(0, 0, 0, 1);

			transformRecursive(child);
		}
	}


	void calculateU_i() {
		if (parent_index == -1) {
            B_ji = glm::mat4({{1,                                0, 0, 0},
                       {0,                                1, 0, 0},
                       {0,                                0, 1, 0},
                       {position, 1}});
			U_i = B_ji * T_i;
			D_i = U_i;
		}

        position = U_i * glm::vec4(0, 0, 0, 1);
        orientation = glm::mat4({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
		rel_orientation = orientation;

		for (int i = 0; i < children.size(); i++) {
		    children[i]->B_ji = glm::mat4({{1,                                0, 0, 0},
                                           {0,                                1, 0, 0},
                                           {0,                                0, 1, 0},
                                           {children[i]->position - position, 1}});
			children[i]->U_i = U_i * children[i]->B_ji * children[i]->T_i;
			children[i]->D_i = children[i]->U_i;
			children[i]->calculateU_i();
		}
	}


	int joint_index;
	int parent_index;
	glm::mat4 D_i;
	glm::mat4 T_i;
	glm::mat4 B_ji;
	glm::mat4 U_i;
	glm::vec3 position;             // position of the joint
	glm::fquat orientation;         // rotation w.r.t. initial configuration
	glm::fquat rel_orientation;     // rotation w.r.t. it's parent. Used for animation.
	glm::vec3 init_position;        // initial position of this joint
	glm::vec3 init_rel_position;    // initial relative position to its parent
	std::vector<Joint*> children;

};

struct Bone {
    Bone(Joint *one, Joint *two) :
        parent(one),
        child(two)
        {}

    Joint *parent;
    Joint *child;

    float distance() {
        return std::abs(glm::distance(parent->position, child->position));
    }

    glm::vec3 direction() {
    	return glm::normalize(child->position - parent->position);
    }
};

struct Configuration {
	std::vector<glm::vec3> trans;
	std::vector<glm::fquat> rot;
	std::vector<glm::vec3> axes;
	std::vector<float> angles;

	const auto& angleData() const { return angles; }
	const auto& axesData() const { return axes; }
	const auto& transData() const { return trans; }
	const auto& rotData() const { return rot; }
};

struct KeyFrame {
	std::vector<glm::fquat> rel_rot;
    bool rendered = true;
	TextureToRender texture;
	static void interpolate(const KeyFrame& from,
	                        const KeyFrame& to,
	                        float tau,
	                        KeyFrame& target);
};

struct LineMesh {
	std::vector<glm::vec4> vertices;
	std::vector<glm::uvec2> indices;
};

struct Node {
	Node(Joint joint1)
			: joint(joint1)
	{
	}
	Joint joint;
	std::vector<Node> children;
};

struct Skeleton {
	Skeleton(){};
public:
	std::vector<Joint*> joints;
    	std::vector<Bone*> bones;

	Configuration cache;

	void refreshCache(Configuration* cache = nullptr);
	const glm::vec3* collectJointTrans() const;
	const glm::fquat* collectJointRot() const;

	// FIXME: create skeleton and bone data structures
};

struct Mesh {
	Mesh();
	~Mesh();
	std::vector<glm::vec4> vertices;
	/*
	 * Static per-vertex attrributes for Shaders
	 */
	std::vector<int32_t> joint0;
	std::vector<int32_t> joint1;
	std::vector<float> weight_for_joint0; // weight_for_joint1 can be calculated
	std::vector<glm::vec3> vector_from_joint0;
	std::vector<glm::vec3> vector_from_joint1;
	std::vector<glm::vec4> vertex_normals;
	std::vector<glm::vec4> face_normals;
	std::vector<glm::vec2> uv_coordinates;
	std::vector<glm::uvec3> faces;

	std::vector<Material> materials;
	BoundingBox bounds;
	Skeleton skeleton;
	std::vector<KeyFrame> keyFrames;
	std::chrono::steady_clock *clock;
	std::chrono::time_point<std::chrono::steady_clock> previous = clock->now();
	float totalRunTime = 0;
    GLuint Panel;

	void loadPmd(const std::string& fn);
	int getNumberOfBones() const;
	glm::vec3 getCenter() const { return 0.5f * glm::vec3(bounds.min + bounds.max); }
	const Configuration* getCurrentQ() const; // Configuration is abbreviated as Q
	void updateAnimation(float t = -1.0);

	void saveAnimationTo(const std::string& fn);
	void loadAnimationFrom(const std::string& fn);

//	std::unordered_map<int, Joint> jointsMap;
private:
	void computeBounds();
	void computeNormals();
    Configuration currentQ_;

};


#endif
