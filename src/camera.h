// Copyright [2015] <Chafic Najjar>

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/shader_program.h"

class InputSystem;

class Camera {
 public:
    Camera(ShaderProgram& shader_program, int screen_width, int screen_height);
    ~Camera();

    bool is_rendering() const;

    GLuint get_shader_program() const;

    glm::mat4 get_projection_matrix() const;
    glm::mat4 get_view_matrix() const;
    glm::vec3 get_eye() const;
    glm::vec3 get_focus() const;
    glm::vec3 get_up() const;
    glm::vec3 get_focus_axis() const;
    glm::vec3 get_right_axis() const;

    void set_projection_matrix(float fovy, float aspect, float near, float far);
    void set_view_matrix(glm::mat4 view_matrix);
    void set_eye(glm::vec3 eye);
    void set_focus(glm::vec3 focus);
    void set_up(glm::vec3 up);
    void add_to_eye(glm::vec3 eye);
    void add_to_focus(glm::vec3 focus);

    void strafe_right(float value);
    void strafe_left(float value);
    void move_backward(float value);
    void move_forward(float value);
    void move_upward(float value);
    void move_downward(float value);
    void rotate_pitch(float x_angle);
    void rotate_yaw(float y_angle);
    void rotate_roll(float z_angle);

    void update(InputSystem& input_system);

 private:
    bool is_rendering_;

    float fovy_;
    float aspect_;
    float near_;
    float far_;

    glm::vec3 eye_;
    glm::vec3 focus_;
    glm::vec3 up_;

    ShaderProgram& shader_program_;

    int projection_matrix_location_;
    int view_matrix_location_;

    glm::mat4 projection_matrix_;
    glm::mat4 view_matrix_;

    void refresh();
    void update_shader_view();
};

#endif  // SRC_CAMERA_H_
