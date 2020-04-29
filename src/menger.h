#ifndef MENGER_H
#define MENGER_H

#include <glm/glm.hpp>
#include <vector>

class Menger {
public:
	Menger();
	~Menger();
	void set_nesting_level(int);
	bool is_dirty() const;
	void set_clean();
	void generate_geometry(std::vector<glm::vec4>& obj_vertices,
	                       std::vector<glm::uvec3>& obj_faces) const;

private:
	void findTriangles(std::vector<glm::vec4> cubeVertices, glm::vec4 squareFace, std::vector<glm::uvec3>& obj_faces, double dist, int offset, glm::vec3 normal) const;
	void rightOrder(std::vector<glm::vec4> obj_vertices, glm::uvec3& indexList, glm::vec3 normal) const;
	void create_menger_sponge(std::vector<glm::vec4>& obj_vertices,
                     std::vector<glm::uvec3>& obj_faces, double depth, glm::vec4 leftCorner, glm::vec4 rightCorner) const;
	void create_cube(std::vector<glm::vec4>& cubeVertices,
                    std::vector<glm::uvec3>& obj_faces, glm::vec4 leftCorner, glm::vec4 rightCorner) const; 
	int nesting_level_ = 0;
	bool dirty_ = false;
};

#endif
