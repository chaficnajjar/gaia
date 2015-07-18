// Copyright [2015] <Chafic Najjar>

#ifndef SRC_SHADER_PROGRAM_H_
#define SRC_SHADER_PROGRAM_H_

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <fstream>
#include <iostream>
#include <string>

class ShaderProgram {
 public:
    ShaderProgram(
            const std::string& vertex_shader_file_name,
            const std::string& fragment_shader_file_name);
    ~ShaderProgram();
    std::string load_shader(const std::string& file_name);
    GLuint create_shader(const std::string& text, GLenum shader_type);
    void bind() const;
    void check_shader_error(GLuint shader,
            GLuint flag, bool is_program, const std::string& error_message);
    GLuint get_program() const;

 private:
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint program_;
};

#endif  // SRC_SHADER_PROGRAM_H_
