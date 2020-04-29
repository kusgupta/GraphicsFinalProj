#include <iostream>
#include "menger.h"

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
}

Menger::~Menger()
{
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

void Menger::create_cube(std::vector<glm::vec4> &obj_vertices, std::vector<glm::uvec3> &obj_faces, glm::vec4 leftCorner,
                         glm::vec4 rightCorner) const {
	
	//Used to calculate the indices for the next cube to be added
	int offset = obj_vertices.size();
	//Cube vertices indexed from 0-7
	std::vector<glm::vec4> cubeVertices;

	double dist = rightCorner[0] - leftCorner[0];
	//Generates all vertices and adds them to the list of cube vertices and overall list of vertices
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				glm::vec4 nextVertex(leftCorner[0], leftCorner[1], leftCorner[2], leftCorner[3]);
				//These calculations get all the different combination of coordinates
				if (i == 1) {
					nextVertex += glm::vec4(dist, 0.0, 0.0, 0.0);
				}

				if (j == 1) {
					nextVertex += glm::vec4(0.0, dist, 0.0, 0.0);
				}

				if (k == 1) {
					nextVertex += glm::vec4(0.0, 0.0, dist, 0.0);
				}	

				cubeVertices.push_back(nextVertex);
				obj_vertices.push_back(nextVertex);
			}
		}
	}
	


	//Indices 0-2 are neg. x,y,z normals, 3-5 are positive index normals
	glm::vec3 normals[6];
	std::vector<glm::vec4> squareFaces;
	//Each face in this list contains the index of a vertex in cubeVertices
	
	for (int i = 0; i < 3; i++) {
	//For each face in a negative axis direction
		int vCounter = 0;
		glm::vec4 squareFace;

		for (unsigned int j = 0; j < cubeVertices.size(); j++) {
		//Finds all points on the plane and adds them to a square face
			if (cubeVertices[j][i] == leftCorner[i]) {
				squareFace[vCounter] = j;
				vCounter++;
			}
		}

		for (int j = 0; j < 3; j++) {
		//Computes the corresponding normal, which is negative
			normals[i][j] = 0.0;
		}
		normals[i][i] = -1.0;
		squareFaces.push_back(squareFace);
	}

	for (int i = 0; i < 3; i++) {
	//For each face in a positive axis direction
		int vCounter = 0;
		glm::dvec4 squareFace;
		for (unsigned int j = 0; j < cubeVertices.size(); j++) {
			if (cubeVertices[j][i] == rightCorner[i]) {
				squareFace[vCounter] = j;
				vCounter++;
			}
		}

		for (int j = 0; j < 3; j++) {
			normals[3 + i][j] = 0;
		}
		normals[3 + i][i] = 1;

		squareFaces.push_back(squareFace);
	}

	int j = 0;
	for (glm::vec4 face : squareFaces) {
		findTriangles(cubeVertices, face, obj_faces, dist, offset, normals[j]);
		j++;
	}
}

void Menger::create_menger_sponge(std::vector<glm::vec4>& obj_vertices,
                          std::vector<glm::uvec3>& obj_faces, double depth, glm::vec4 leftCorner, glm::vec4 rightCorner) const {

    if (depth == 0) {
//                std::cout << "depth = " << la << std::endl;

//        std::cout << "depth = " << la << std::endl;
//        std::cout << "x = " << leftCorner[0] << std::endl;
//        std::cout << "y = " << leftCorner[1] << std::endl;
//        std::cout << "z = " << leftCorner[2] << std::endl;
//        std::cout << "x = " << rightCorner[0] << std::endl;
//        std::cout << "y = " << rightCorner[1] << std::endl;
//        std::cout << "z = " << rightCorner[2] << std::endl;
        Menger::create_cube(obj_vertices, obj_faces, leftCorner, rightCorner);
    }
    else {
	//Distances to iterate when finding the left and right corners of each sub-cube
        float move_distance_x = (rightCorner[0] - leftCorner[0]) / 3.0;
        float move_distance_y = (rightCorner[1] - leftCorner[1]) / 3.0;
        float move_distance_z = (rightCorner[2] - leftCorner[2]) / 3.0;
        int x_num = 0;
        int y_num = 0;
        int z_num = 0;
	//Iterates through all 27 sub-cubes
		int count = 0;
		float x, y, z;
		int i, j, k;
        for (x = leftCorner[0], i = 0; i < 3; x+=move_distance_x, i++) {
            for (y = leftCorner[1], j = 0; j < 3; y+=move_distance_y, j++) {
                for (z = leftCorner[2], k = 0; k < 3; z+=move_distance_z, k++) {
                    count++;
					if (!(x_num == 1 && y_num == 1) && !(x_num == 1 && z_num == 1) && !(y_num == 1 && z_num == 1)) {
//                  //Excludes all inner sub-cubes
//                      la +=1 ;
						
                        create_menger_sponge(obj_vertices, obj_faces, depth - 1,
                                             glm::vec4(x, y, z, 1),
                                             glm::vec4(x + move_distance_x, y + move_distance_y, z + move_distance_z,
                                                       1));
                    }
                    z_num++;
                }
                z_num = 0;
                y_num++;
            }
            y_num = 0;
            x_num++;
        }
		std::cout << count << std::endl;
    }
}

// FIXME generate Menger sponge geometry
void Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) const
{
	create_menger_sponge(obj_vertices, obj_faces, Menger::nesting_level_,glm::dvec4(-.5, -.5, -.5, 1), glm::dvec4(.5, .5, .5, 1));
}

void Menger::findTriangles(std::vector<glm::vec4> cubeVertices, glm::vec4 squareFace, std::vector<glm::uvec3>& obj_faces, double dist, int offset, glm::vec3 normal) const {
	//For each face, adds the 2 triangles in the correct order to the list of faces.
	int vertexToExclude = -1;
	for (int i = 0; i < 3; i++) {
	//Among the first 3 vertices, finds the side that isn't the same length as the cube length. We know which vertex is the right angle corner.
		glm::vec4 v1 = cubeVertices[(int) squareFace[i]];
		glm::vec4 v2 = cubeVertices[(int) squareFace[(i + 1) % 3]];

		glm::vec3 vertex1(v1[0], v1[1], v1[2]);
		glm::vec3 vertex2(v2[0], v2[1], v2[2]);

		if (glm::distance(vertex1, vertex2) != dist) {
			//Now we know where the first face is.
			vertexToExclude = (i + 2) % 3;
			int v0index = offset + squareFace[i];
			int v1index = offset + squareFace[(i + 2) % 3];
			int v2index = offset + squareFace[(i + 1) % 3];

			glm::uvec3 firstFace(v0index, v1index, v2index);
			
			rightOrder(cubeVertices, firstFace, normal);
			obj_faces.push_back(firstFace);
			
			break;
		}
	}

	//Now we find the other face to draw
	glm::uvec3 otherFace;
	int vCounter = 0;
	for (int i = 0; i < 3; i++) {
		//Pick the 2 vertices that aren't the first right corner, and find the other right corner
		if (i != vertexToExclude) {
			otherFace[vCounter] = offset + squareFace[i];
			vCounter += 2;
		}
	}

	otherFace[1] = offset + squareFace[3];
	rightOrder(cubeVertices, otherFace, normal);

	obj_faces.push_back(otherFace);


}

void Menger::rightOrder(std::vector<glm::vec4> cubeVertices, glm::uvec3& indexList, glm::vec3 normal) const {
//Will re-order vertices if they are in the wrong order with respect to the normal.
	
	// glm::vec4 s1 =  cubeVertices[indexList[1]] - cubeVertices[indexList[0]];
	// glm::vec4 s2 =  cubeVertices[indexList[1]] - cubeVertices[indexList[2]];

	// glm::vec3 side1 = glm::uvec3(s1[0], s1[1], s1[2]);
	// glm::vec3 side2 = glm::uvec3(s2[0], s2[1], s2[2]);

	// //Cross-product helps find what direction the normal is in. If the dot with the normal is non-positive, we re-align the non-right corner vertices.
	// if (glm::dot(glm::cross(side2, side1), normal) < 0) {
	// 	double temp = indexList[2];
	// 	indexList[2] = indexList[0];
	// 	indexList[0] = temp;
	// }
}


