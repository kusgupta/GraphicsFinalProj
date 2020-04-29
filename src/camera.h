#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix();
	// FIXME: add functions to manipulate camera objects.
	void forward_backward(bool forward);
	void strafe(bool right);
	void roll(bool right);
	void up_down(bool up);
	void drag(glm::vec3 position);
private:
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
    void setRowVia3DVec(glm::mat4 &matrix, glm::vec3 &vec, int row) const;
    glm::vec3 prev_position = glm::vec3(0, 0, 0);
};

#endif
