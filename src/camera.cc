// Copyright [2015] <Chafic Najjar>

#include "src/camera.h"
#include "src/input_system.h"

Camera::Camera(ShaderProgram& shader_program, int screen_width, int screen_height) :
    is_rendering_(true),
    fovy_(60.0f),
    aspect_(static_cast<float>(screen_width)/static_cast<float>(screen_height)),
    near_(0.1f),
    far_(100.0f),
    eye_(glm::vec3(0.0f, 0.0f, 3.0f)),
    focus_(glm::vec3(eye_ + glm::vec3(0.0f, 0.0f, -1.0f))),
    up_(glm::vec3(0.0f, 1.0f, 0.0f)),

    shader_program_(shader_program),

    projection_matrix_location_(
            glGetUniformLocation(get_shader_program(), "projection_matrix")),

    view_matrix_location_(glGetUniformLocation(get_shader_program(), "view_matrix")),

    projection_matrix_(glm::perspective(glm::radians(fovy_), aspect_, near_, far_)),

    view_matrix_(glm::lookAt(eye_, focus_, up_)) {
    glUseProgram(shader_program_.get_program());
    glUniformMatrix4fv(
            projection_matrix_location_,
            1,
            GL_FALSE,
            glm::value_ptr(projection_matrix_));
}


Camera::~Camera() {}

void Camera::refresh() {
    view_matrix_ = glm::lookAt(eye_, focus_, up_);
}

void Camera::update(InputSystem& input_system) {
    if (is_rendering()) {
        rotate_yaw(input_system.get_delta_x()/10.0f);
        rotate_pitch(input_system.get_delta_y()/10.0f);

        while (!input_system.actions_.empty()) {
            InputSystem::ActionNumber next_action = input_system.actions_.front();
            input_system.actions_.pop();

            if (next_action == InputSystem::MOVE_FORWARD) {
                move_forward(0.1f);
            } else if (next_action == InputSystem::MOVE_BACKWARD) {
                move_backward(0.1f);
            } else if (next_action == InputSystem::MOVE_LEFT) {
                strafe_left(0.1f);
            } else if (next_action == InputSystem::MOVE_RIGHT) {
                strafe_right(0.1f);
            } else if (next_action == InputSystem::MOVE_UPWARD) {
                move_upward(0.1f);
            } else if (next_action == InputSystem::MOVE_DOWNWARD) {
                move_downward(0.1f);
            }
        }

        update_shader_view();
    }
}

void Camera::update_shader_view() {
    // &view_matrix[0][0] also works.
    glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, glm::value_ptr(view_matrix_));
}


bool Camera::is_rendering() const {
    return is_rendering_;
}


GLuint Camera::get_shader_program() const {
    return shader_program_.get_program();
}


glm::mat4 Camera::get_projection_matrix() const {
    return projection_matrix_;
}


glm::mat4 Camera::get_view_matrix() const {
    return view_matrix_;
}


glm::vec3 Camera::get_eye() const {
    return eye_;
}


glm::vec3 Camera::get_focus() const {
    return focus_;
}

glm::vec3 Camera::get_up() const {
    return up_;
}


glm::vec3 Camera::get_focus_axis() const {
    return glm::normalize(focus_ - eye_);
}


glm::vec3 Camera::get_right_axis() const {
    return glm::normalize(glm::cross(get_focus_axis(), up_));
}


void Camera::set_projection_matrix(float fovy, float aspect, float near, float far) {
    projection_matrix_ = glm::perspective(glm::radians(fovy_), aspect_, near_, far_);

    // &projection_matrix[0][0] also works
    glUniformMatrix4fv(
            projection_matrix_location_,
            1,
            GL_FALSE,
            glm::value_ptr(projection_matrix_));
}


void Camera::set_view_matrix(glm::mat4 view_matrix) {
    view_matrix_ = view_matrix;
}


void Camera::set_eye(glm::vec3 eye) {
    eye_ = eye;
}


void Camera::set_focus(glm::vec3 focus) {
    if (std::abs(glm::dot(up_, glm::normalize(focus - eye_))) < 0.97)
        focus_ = focus;
}


void Camera::set_up(glm::vec3 up) {
    up_ = up;
}


void Camera::add_to_eye(glm::vec3 eye) {
    eye_ += eye;
}


void Camera::add_to_focus(glm::vec3 focus) {
    focus_ += focus;
}

void Camera::strafe_right(float value) {
    // Get right axis
    glm::vec3 right_axis = get_right_axis();

    // Multiply by value
    glm::vec3 translation_vector = right_axis * value;

    // Set eye and focus
    add_to_eye(translation_vector);
    add_to_focus(translation_vector);
    refresh();
}


void Camera::strafe_left(float value) {
    // Get right axis
    glm::vec3 right_axis = get_right_axis();

    // Multiply by value
    glm::vec3 translation_vector = right_axis * value;

    // Set eye and focus
    add_to_eye(-translation_vector);
    add_to_focus(-translation_vector);
    refresh();
}


void Camera::move_backward(float value) {
    // Get focus axis
    glm::vec3 focus_axis = get_focus_axis();

    // Multiply by value
    glm::vec3 translation_vector = focus_axis * value;

    // Set eye and focus
    add_to_eye(-translation_vector);
    add_to_focus(-translation_vector);
    refresh();
}


void Camera::move_forward(float value) {
    // Get focus axis
    glm::vec3 focus_axis = get_focus_axis();

    // Multiply by value
    glm::vec3 translation_vector = focus_axis * value;

    // Set eye and focus
    add_to_eye(translation_vector);
    add_to_focus(translation_vector);
    refresh();
}


void Camera::move_upward(float value) {
    // Multiply by value
    glm::vec3 translation_vector = get_up() * value;

    // Set eye and focus
    add_to_eye(translation_vector);
    add_to_focus(translation_vector);
    refresh();
}


void Camera::move_downward(float value) {
    // Multiply by value
    glm::vec3 translation_vector = get_up() * value;

    // Set eye and focus
    add_to_eye(-translation_vector);
    add_to_focus(-translation_vector);
    refresh();
}


// Look up or down
void Camera::rotate_pitch(float x_angle) {
    // Get 'focus' axis
    glm::vec3 focus_axis = get_focus_axis();

    // Create rotation matrix
    glm::mat4 rotation_matrix;
    rotation_matrix = glm::rotate(
            rotation_matrix,
            glm::radians(x_angle),
            get_right_axis());

    // Apply rotation to 'focus' axis and set camera 'focus'
    // point in world space.
    glm::vec4 focus_axis_result =
        glm::normalize(rotation_matrix * glm::vec4(focus_axis, 1.0f));

    set_focus(get_eye() + glm::vec3(focus_axis_result));

    refresh();
}


// Look right or left
void Camera::rotate_yaw(float y_angle) {
    // Get 'focus' axis
    glm::vec3 focus_axis = get_focus_axis();

    // Create rotation matrix
    glm::mat4 rotation_matrix;

    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(y_angle), get_up());

    // Apply rotation to camera's 'focus' axis
    glm::vec4 focus_axis_result = glm::normalize(
            rotation_matrix * glm::vec4(focus_axis, 1.0f));

    set_focus(get_eye() + glm::vec3(focus_axis_result));

    refresh();
}


// Rotate fixed camera sideways
void Camera::rotate_roll(float z_angle) {
    glm::mat4 rotation_matrix = glm::rotate(
            rotation_matrix,
            glm::radians(z_angle),
            get_focus_axis());

    refresh();
}
