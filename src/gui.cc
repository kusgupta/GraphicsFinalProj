#include "gui.h"
#include "config.h"
#include <jpegio.h>
#include "bone_geometry.h"
#include <iostream>
#include <algorithm>
#include <debuggl.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace {
	// FIXME: Implement a function that performs proper
	//        ray-cylinder intersection detection
	// TIPS: The implement is provided by the ray-tracer starter code.
}

class ray {


public:
    ray(glm::vec4 p, glm::vec4 d) {
        position = p;
        direction = d;
    }

    glm::vec4 position;
    glm::vec4 direction;

};

GUI::GUI(GLFWwindow* window, int view_width, int view_height, int preview_height)
	:window_(window), preview_height_(preview_height)
{
	glfwSetWindowUserPointer(window_, this);
	glfwSetKeyCallback(window_, KeyCallback);
	glfwSetCursorPosCallback(window_, MousePosCallback);
	glfwSetMouseButtonCallback(window_, MouseButtonCallback);
	glfwSetScrollCallback(window_, MouseScrollCallback);

	glfwGetWindowSize(window_, &window_width_, &window_height_);
	if (view_width < 0 || view_height < 0) {
		view_width_ = window_width_;
		view_height_ = window_height_;
	} else {
		view_width_ = view_width;
		view_height_ = view_height;
	}
	float aspect_ = static_cast<float>(view_width_) / view_height_;
	projection_matrix_ = glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
}

GUI::~GUI()
{
}

void GUI::assignMesh(Mesh* mesh)
{
	mesh_ = mesh;
	center_ = mesh_->getCenter();
}

void GUI::keyCallback(int key, int scancode, int action, int mods)
{
#if 0
	if (action != 2)
		std::cerr << "Key: " << key << " action: " << action << std::endl;
#endif
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, GL_TRUE);
		return ;
	}
	if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
		//FIXME save out a screenshot using SaveJPEG
	}
	if (key == GLFW_KEY_S && (mods & GLFW_MOD_CONTROL)) {
		if (action == GLFW_RELEASE)
			mesh_->saveAnimationTo("animation.json");
		return ;
	}

	if (mods == 0 && captureWASDUPDOWN(key, action))
		return ;
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
		float roll_speed;
		if (key == GLFW_KEY_RIGHT)
			roll_speed = -roll_speed_;
		else
			roll_speed = roll_speed_;
		// FIXME: actually roll the bone here
	} else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		fps_mode_ = !fps_mode_;
	} else if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_RELEASE) {
		current_bone_--;
		current_bone_ += mesh_->getNumberOfBones();
		current_bone_ %= mesh_->getNumberOfBones();
	} else if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_RELEASE) {
		current_bone_++;
		current_bone_ += mesh_->getNumberOfBones();
		current_bone_ %= mesh_->getNumberOfBones();
	} else if (key == GLFW_KEY_T && action != GLFW_RELEASE) {
		transparent_ = !transparent_;
	}

	// FIXME: implement other controls here.
}

void GUI::mousePosCallback(double mouse_x, double mouse_y)
{
    last_x_ = current_x_;
    last_y_ = current_y_;
    current_x_ = mouse_x;
    current_y_ = window_height_ - mouse_y;
    float delta_x = current_x_ - last_x_;
    float delta_y = current_y_ - last_y_;
    if (sqrt(delta_x * delta_x + delta_y * delta_y) < 1e-15)
        return;
//    if (mouse_x > view_width_ && current_button_ == GLFW_MOUSE_BUTTON_LEFT) {
//        KeyFrame k;
//        // frame0
//        if (mouse_y > 480 && mouse_y <= 720 && mesh_->keyFrames.size() > 2) {
//            currentSelectedKeyframe = scrollBar + 2;
//        } else if (mouse_y > 240 && mouse_y <= 480 && mesh_->keyFrames.size() > 1) {
//            currentSelectedKeyframe = scrollBar + 1;
//        } else if (mouse_y >=0 && mouse_y <= 240) {
//            currentSelectedKeyframe = scrollBar;
//        }
//    }
    glm::vec3 mouse_direction = glm::normalize(glm::vec3(delta_x, delta_y, 0.0f));
    glm::vec2 mouse_start = glm::vec2(last_x_, last_y_);
    glm::vec2 mouse_end = glm::vec2(current_x_, current_y_);
    glm::uvec4 viewport = glm::uvec4(0, 0, view_width_, view_height_);

    bool drag_camera = drag_state_ && current_button_ == GLFW_MOUSE_BUTTON_RIGHT;
    bool drag_bone = drag_state_ && current_button_ == GLFW_MOUSE_BUTTON_LEFT;
    if (drag_bone) {

    }
    if (isPlaying()) {
        return;
    }
    if (drag_camera) {
        glm::vec3 axis = glm::normalize(
                orientation_ *
                glm::vec3(mouse_direction.y, -mouse_direction.x, 0.0f)
        );
        orientation_ =
                glm::mat3(glm::rotate(rotation_speed_, axis) * glm::mat4(orientation_));
        tangent_ = glm::column(orientation_, 0);
        up_ = glm::column(orientation_, 1);
        look_ = glm::column(orientation_, 2);
    } else if (drag_bone && current_bone_ != -1) {
        // FIXME: Handle bone rotation
        glm::vec4 mouse_world_current = glm::inverse(projection_matrix_) *
                                        glm::vec4(2 * (current_x_ / view_width_ - .5),
                                                  2 * (current_y_ / view_height_ - .5), -1, 1);
        glm::vec4 mouse_world_prev = glm::inverse(projection_matrix_) *
                                     glm::vec4(2 * (last_x_ / view_width_ - .5), 2 * (last_y_ / view_height_ - .5), -1,
                                               1);
//        glm::vec4 mouse_direction_world = glm::normalize(mouse_world_prev - mouse_world_current);
//        float angle = glm::cos(mouse_direction

        mouse_world_current = mouse_world_current / mouse_world_current[3];
        mouse_world_current = glm::inverse(view_matrix_) * mouse_world_current;

        mouse_world_prev = mouse_world_prev / mouse_world_prev[3];
        mouse_world_prev = glm::inverse(view_matrix_) * mouse_world_prev;

        ray r1(glm::vec4({eye_, 1}),
               {normalize(glm::vec3(mouse_world_current[0], mouse_world_current[1], mouse_world_current[2]) - eye_),
                1});
        ray r2(glm::vec4({eye_, 1}),
               {normalize(glm::vec3(mouse_world_prev[0], mouse_world_prev[1], mouse_world_prev[2]) - eye_), 1});

        Bone *curBone = mesh_->skeleton.bones[current_bone_];
        glm::vec3 q = curBone->parent->position;


        float t_current = (glm::dot((q - eye_), -look_)) / (glm::dot(r1.direction, glm::vec4(-look_, 0)));
        float t_prev = (glm::dot((q - eye_), -look_)) / (glm::dot(r2.direction, glm::vec4(-look_, 0)));

        //Calculates the intersection coordinates with parent as origin
        glm::vec3 intersectionPoint_current = glm::normalize(eye_ + glm::vec3(r1.direction) * t_current - q);
        glm::vec3 intersectionPoint_prev = glm::normalize(eye_ + glm::vec3(r2.direction) * t_prev - q);

        float angle1, angle2;

        //Based on angles with x and y-axis, finds the angle
        float xAngle1 = glm::acos(glm::dot(intersectionPoint_current, tangent_));
        float yAngle1 = glm::acos(glm::dot(intersectionPoint_current, up_));

        if (yAngle1 > 1.57 && xAngle1 > 1.57) {
            //3rd quadrant
            angle1 = 3.14 + xAngle1;
        } else if (yAngle1 < 1.57 && xAngle1 < 1.57) {
            //Quadrant 1
            angle1 = xAngle1;
        } else if (yAngle1 < 1.57 && xAngle1 > 1.57) {
            //Quadrant 2
            angle1 = 3.14 - xAngle1;
        } else if (yAngle1 > 1.57 && xAngle1 < 1.57) {
            //Quadrant 4
            angle1 = 6.28 - xAngle1;
        }

        float xAngle2 = glm::acos(glm::dot(intersectionPoint_prev, tangent_));
        float yAngle2 = glm::acos(glm::dot(intersectionPoint_prev, up_));

        if (yAngle2 > 1.57 && xAngle2 > 1.57) {
            angle2 = 3.14 + xAngle2;
        } else if (yAngle2 < 1.57 && xAngle2 < 1.57) {
            angle2 = xAngle2;
        } else if (yAngle2 < 1.57 && xAngle2 > 1.57) {
            angle2 = 3.14 - xAngle2;
        } else if (yAngle2 > 1.57 && xAngle2 < 1.57) {
            angle2 = 6.28 - xAngle2;
        }

        //Angle to rotate by
        float angle = angle1 - angle2;


        //Gets the transformation to rotate by to change the T_i
        glm::mat4 transform = glm::rotate(angle, look_);
        glm::fquat quat(curBone->parent->T_i);




        //std::cout << glm::to_string(transform) << std::endl;

        if (curBone->parent->parent_index != -1) {
            curBone->parent->applyTransformations(curBone->parent,
                                                  mesh_->skeleton.joints[curBone->parent->parent_index], transform);
        } else {
            curBone->parent->applyTransformations(curBone->parent,
                                                  nullptr, transform);
        }
        curBone->parent->transformRecursive(curBone->parent);
        mesh_->skeleton.refreshCache(const_cast<Configuration *>(mesh_->getCurrentQ()));
        return;
    }

    //This converts the mouse coordinates into world coordiantes
    glm::vec4 mouse_world = glm::inverse(projection_matrix_) *
                            glm::vec4(2 * (current_x_ / view_width_ - .5), 2 * (current_y_ / view_height_ - .5), -1, 1);

    mouse_world = mouse_world / mouse_world[3];
    mouse_world = glm::inverse(view_matrix_) * mouse_world;


    ray r(glm::vec4({eye_, 1}), {normalize(glm::vec3(mouse_world[0], mouse_world[1], mouse_world[2]) - eye_), 1});

    std::vector<Bone *> bones = mesh_->skeleton.bones;

    std::vector<Bone *> intersectedWith;

    float earliestT = 10000000.0;
    int bone_index = -1;
    for (int i = 0; i < bones.size(); i++) {
        //For each bone, find if it intersects the ray and if it has the earliest intersection
        Bone *currentBone = bones[i];


        float distance = currentBone->distance();
        glm::vec3 yDirection = currentBone->direction();
        glm::vec3 zDirection;
        if (yDirection[0] == 0 && yDirection[1] == 0) {
            zDirection = glm::vec3(1, 0, 0);
        } else {
            zDirection = glm::normalize(glm::vec3(-yDirection[1], yDirection[0], 0));
        }
        glm::vec3 xDirection = glm::normalize(glm::cross(yDirection, zDirection));

        float height = distance;
        glm::mat4 cylinder_transform = glm::inverse(glm::mat4({{xDirection, 0},
                                                               {yDirection, 0},
                                                               {zDirection, 0},
                                                               {0,          0, 0, 1}}))
                                       *
                                       glm::mat4({{1,                                  0, 0, 0},
                                                  {0,                                  1, 0, 0},
                                                  {0,                                  0, 1, 0},
                                                  {currentBone->parent->position.x * -1, currentBone->parent->position.y * -1, currentBone->parent->position.z * -1, 1}});

        glm::mat4 coordinate_transform = glm::inverse(glm::mat4({{xDirection, 0},
                                                                 {yDirection, 0},
                                                                 {zDirection, 0},
                                                                 {0,          0, 0, 1}}));

        ray local(cylinder_transform * r.position, coordinate_transform * r.direction);


        glm::vec2 p(local.position.x, local.position.z);
        glm::vec2 v(local.direction.x, local.direction.z);
        glm::vec2 vNorm(glm::normalize(v));
        float k = kCylinderRadius * kCylinderRadius + glm::dot(p, vNorm) * glm::dot(p, vNorm) - glm::dot(p, p);

        if (k < 0) {
            continue;
        } else {
            float t_plus = glm::sqrt(k) - glm::dot(p, vNorm);
            float t_minus = -glm::sqrt(k) - glm::dot(p, vNorm);
            float m = glm::sqrt(v.x * v.x + v.y * v.y);

            if ((t_plus > 0 && t_minus < 0) || (t_minus > 0 && t_plus < 0)) {
                float t1 = 0;
                float t2;
                if (t_plus > 0) {
                    t2 = t_plus;
                } else {
                    t2 = t_minus;
                }


                float y1 = local.position.y + (t1 * local.direction / m).y;
                float y2 = local.position.y + (t2 * local.direction / m).y;

                if (y2 < y1) {
                    float temp = y2;
                    y2 = y1;
                    y1 = temp;
                }

                float parentY = 0;
                float childY = distance;

                if (parentY > y2 || y1 > childY) {
                    //If the intervals don't intersect, don't do anything
                    continue;
                }

                float endpoints[] = {y1, y2, parentY, childY};
                std::sort(endpoints, endpoints + 4);

                float posY1 = endpoints[1];
                float posY2 = endpoints[2];

                float posT1 = (posY1 - local.position.y) / local.direction.y;
                float posT2 = (posY2 - local.position.y) / local.direction.y;

                if (earliestT != fminf(earliestT, fminf(posT1, posT2))) {
                    std::cout << "entered" << std::endl;
                    bone_index = i;
                    earliestT = fminf(earliestT, fminf(posT1, posT2));
                }


            } else if (t_plus > 0 && t_minus > 0) {
                float t1;
                float t2;
                if (t_plus > t_minus) {
                    t1 = t_plus;
                    t2 = t_minus;
                } else {
                    t1 = t_minus;
                    t2 = t_plus;
                }
                float y1 = local.position.y + (t1 * local.direction / m).y;
                float y2 = local.position.y + (t2 * local.direction / m).y;

                if (y2 < y1) {
                    float temp = y2;
                    y2 = y1;
                    y1 = temp;
                }

                float parentY = 0;
                float childY = distance;

                if (parentY > y2 || y1 > childY) {
                    continue;
                }

                float endpoints[] = {y1, y2, parentY, childY};
                std::sort(endpoints, endpoints + 4);

                float posY1 = endpoints[1];
                float posY2 = endpoints[2];

                float posT1 = (posY1 - local.position.y) / local.direction.y;
                float posT2 = (posY2 - local.position.y) / local.direction.y;

                if (earliestT != fminf(earliestT, fminf(posT1, posT2))) {
//                    std::cout << "entered" << std::endl;
                    bone_index = i;
                    earliestT = fminf(earliestT, fminf(posT1, posT2));
                }

            } else {
                continue;
            }
        }

    }
//    if (bone_index != -1) {
//        std::cout << glm::to_string(mesh_->skeleton.bones[bone_index]->parent->position) << std::endl;
//        std::cout << bone_index << std::endl;}
//    std::cout << glm::to_string(mouse_world) << std::endl;

    setCurrentBone(bone_index);


}

void GUI::mouseButtonCallback(int button, int action, int mods)
{
	if (current_x_ <= view_width_) {
		drag_state_ = (action == GLFW_PRESS);
		current_button_ = button;
		return ;
	}
	// FIXME: Key Frame Selection
}

void GUI::mouseScrollCallback(double dx, double dy)
{
	if (current_x_ < view_width_)
		return;
	// FIXME: Mouse Scrolling
}

void GUI::updateMatrices()
{
	// Compute our view, and projection matrices.
	if (fps_mode_)
		center_ = eye_ + camera_distance_ * look_;
	else
		eye_ = center_ - camera_distance_ * look_;

	view_matrix_ = glm::lookAt(eye_, center_, up_);
	light_position_ = glm::vec4(eye_, 1.0f);

	aspect_ = static_cast<float>(view_width_) / view_height_;
	projection_matrix_ =
		glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	model_matrix_ = glm::mat4(1.0f);
}

MatrixPointers GUI::getMatrixPointers() const
{
	MatrixPointers ret;
	ret.projection = &projection_matrix_;
	ret.model= &model_matrix_;
	ret.view = &view_matrix_;
	return ret;
}

bool GUI::setCurrentBone(int i)
{
	if (i < 0 || i >= mesh_->getNumberOfBones())
		return false;
	current_bone_ = i;
	return true;
}

float GUI::getCurrentPlayTime() const
{
	return 0.0f;
}


bool GUI::captureWASDUPDOWN(int key, int action)
{
	if (key == GLFW_KEY_W) {
		if (fps_mode_)
			eye_ += zoom_speed_ * look_;
		else
			camera_distance_ -= zoom_speed_;
		return true;
	} else if (key == GLFW_KEY_S) {
		if (fps_mode_)
			eye_ -= zoom_speed_ * look_;
		else
			camera_distance_ += zoom_speed_;
		return true;
	} else if (key == GLFW_KEY_A) {
		if (fps_mode_)
			eye_ -= pan_speed_ * tangent_;
		else
			center_ -= pan_speed_ * tangent_;
		return true;
	} else if (key == GLFW_KEY_D) {
		if (fps_mode_)
			eye_ += pan_speed_ * tangent_;
		else
			center_ += pan_speed_ * tangent_;
		return true;
	} else if (key == GLFW_KEY_DOWN) {
		if (fps_mode_)
			eye_ -= pan_speed_ * up_;
		else
			center_ -= pan_speed_ * up_;
		return true;
	} else if (key == GLFW_KEY_UP) {
		if (fps_mode_)
			eye_ += pan_speed_ * up_;
		else
			center_ += pan_speed_ * up_;
		return true;
	}
	return false;
}


// Delegrate to the actual GUI object.
void GUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->keyCallback(key, scancode, action, mods);
}

void GUI::MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mousePosCallback(mouse_x, mouse_y);
}

void GUI::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseButtonCallback(button, action, mods);
}

void GUI::MouseScrollCallback(GLFWwindow* window, double dx, double dy)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseScrollCallback(dx, dy);
}
