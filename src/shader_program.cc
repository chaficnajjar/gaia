// Copyright [2015] <Chafic Najjar>

#include "src/shader_program.h"

ShaderProgram::ShaderProgram(
        const std::string& vertex_shader_file_name,
        const std::string& fragment_shader_file_name) {
    // Create shader program.
    program_ = glCreateProgram();

    // Create vertex and fragment shaders.
    vertex_shader_ = create_shader(
        load_shader(vertex_shader_file_name),
        GL_VERTEX_SHADER);

    fragment_shader_ = create_shader(
        load_shader(fragment_shader_file_name),
        GL_FRAGMENT_SHADER);

    // Link shader program.
    glAttachShader(program_, vertex_shader_);
    glAttachShader(program_, fragment_shader_);
    glLinkProgram(program_);

    // Detach and delete unneeded shaders after linking the shader program.
    glDetachShader(program_, vertex_shader_);
    glDetachShader(program_, fragment_shader_);
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_);
}

GLuint ShaderProgram::create_shader(const std::string& text, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0) {
        std::cerr << "Error: Shader creation failed" << std::endl;
    }

    const GLchar* shaders_source_strings[1];
    GLint shaders_source_string_lengths[1];

    shaders_source_strings[0] = text.c_str();
    shaders_source_string_lengths[0] = text.length();

    glShaderSource(shader, 1, shaders_source_strings, shaders_source_string_lengths);
    glCompileShader(shader);

    check_shader_error(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

    return shader;
}

std::string ShaderProgram::load_shader(const std::string& file_name) {
    std::ifstream file;
    file.open((file_name).c_str());

    std::string output;
    std::string line;

    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << file_name << std::endl;
    }

    return output;
}

void ShaderProgram::bind() const {
    glUseProgram(program_);
}

void ShaderProgram::check_shader_error(GLuint shader, GLuint flag,
        bool is_program, const std::string& error_message) {
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if (is_program) {
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE) {
        if (is_program) {
            // Can I use nullptr instead?
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }

        std::cerr << error_message << ": " << error << std::endl;
    }
}

GLuint ShaderProgram::get_program() const {
    return program_;
}
