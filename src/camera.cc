#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "camera.h"

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

void Camera::setRowVia3DVec(glm::mat4 &matrix, glm::vec3 &vec, int row) const {
    for (int i = 0; i < 3; i++) {
        matrix[row][i] = vec[i];
    }
}

// FIXME: Calculate the view matrix
// I removed const from this method, just a note in case it breaks it
glm::mat4 Camera::get_view_matrix() {
//    glm::vec3 view_look = glm::vec3(-1 * look_[0], -1 * look_[1], -1 * look_[2]);
//    glm::vec3 center = eye_ + camera_distance_ * look_;

    glm::vec3 right = glm::normalize(glm::cross(look_, up_));
    glm::vec3 center = eye_ + camera_distance_ * look_;
    glm::vec3 view_look = glm::vec3(-1 * look_[0], -1 * look_[1], -1 * look_[2]);
    glm::mat4 matrixOne(1.0);
    setRowVia3DVec(matrixOne, right, 0);       
    setRowVia3DVec(matrixOne, up_, 1);
    setRowVia3DVec(matrixOne, view_look, 2);

//    glm::mat4 matrixTwo(1.0);
//    matrixTwo[3][0] = -1.0 * eye_[0];
//    matrixTwo[3][1] = -1.0 * eye_[1];
//    matrixTwo[3][2] = -1.0 * eye_[2];
    setRowVia3DVec(matrixOne, eye_, 3);
//    std::cout << "hi" << std::endl;
//    std::cout << glm::to_string(matrixOne) << std::endl;
//    std::cout << glm::to_string(matrixTwo) << std::endl;

//    auto m = (matrixOne * (matrixTwo));
//    m[3][2] = -3;
//    std::cout << glm::to_string(m) << std::endl;
//    std::cout << "bye" << std::endl;
//    std::cout << "lookat" << std::endl;
//    std::cout << glm::to_string(glm::inverse(glm::lookAt(eye_, center, up_))) << std::endl;
//    std::cout << "me" << std::endl;
//    std::cout << glm::to_string(matrixOne) << std::endl;
//    std::cout << glm::to_string(eye_) << std::endl;

    return glm::inverse(matrixOne);
//    glm::rotate(camera, tilt, pitchVector)
//    return glm::mat4(1.0);
}

void Camera::forward_backward(bool forward) {
    auto translation_amount = zoom_speed * look_;
    if (forward) {
        eye_ += translation_amount;

    }
    else {
        eye_ -= translation_amount;
    }
}

void Camera::strafe(bool right) {
    auto translation_amount = glm::normalize(glm::cross(eye_, up_)) * pan_speed;
    if (right) {
        eye_ -= translation_amount;

    }
    else {
        eye_ += translation_amount;
    }
}

void Camera::roll(bool right) {
//    auto translation_amount = roll_speed * glm::normalize();
//    auto translation_amount2 = roll_speed * glm::normalize(up_);
//    std::cout << glm::to_string(translation_amount) << std::endl;
//    std::cout << glm::to_string(look_) << std::endl;
//    glm::mat4 trans = glm::mat4(1.0);

    if (right) {

        up_ = glm::rotate(up_, roll_speed, look_);
//        glm::mat4 trans = glm::mat4(1.0f);
//        trans = glm::rotate(trans, roll_speed, look_);
//        glm::vec4 r(up_[0], up_[1], up_[2], 1);
//        glm::vec4 result = trans * r;
//        up_[0] = result[0];
//        up_[1] = result[1];
//        up_[2] = result[2];
//        std::cout << result[3] << std::endl;

//        up_  -=translation_amount2;
        std::cout << glm::to_string(up_) << std::endl;
    }
    else {
        up_ = glm::rotate(up_, -1 * roll_speed, look_);
//        glm::mat4 model = glm::mat4(1.0f);

//        model = glm::translate(model,glm::vec3(0,0,0)); //position = 0,0,0
//        model = glm::rotate(model,glm::radians(0.0f),glm::vec3(1,0,0));
//        look_ += translation_amount;
        std::cout << glm::to_string(up_) << std::endl;

    }
}

void Camera::up_down(bool up) {
    auto translation_amount = pan_speed * up_;
    if (up) {
        eye_ += translation_amount;

    }
    else {
        eye_ -= translation_amount;
    }
}

void Camera::drag(glm::vec3 position) {
    if (prev_position != glm::vec3(0, 0, 0)) {
        auto direction =  glm::normalize(prev_position - position);
        auto axis = glm::cross(direction, look_);
        look_ = glm::rotate(look_, rotation_speed, -axis);
    }
    prev_position = position;
}









