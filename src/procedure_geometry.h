#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>

struct LineMesh;

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
void create_more_floors(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces, int add_face, int add_x, int add_y, int add_z);
void create_bone_mesh(LineMesh& bone_mesh);
void create_cylinder_mesh(LineMesh& cylinder_mesh);
void create_axes_mesh(LineMesh& axes_mesh);

#endif
