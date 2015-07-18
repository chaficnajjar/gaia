// Copyright [2015] <Chafic Najjar>

#include "src/graphics_system.h"

#include "src/shader_program.h"
#include "src/mesh.h"

GraphicsSystem::GraphicsSystem(int screen_width, int screen_height) :
    window_title_("Gaia"),
    screen_width_(screen_width),
    screen_height_(screen_height) {
    // Initialize SDL.
    SDL_Init(SDL_INIT_EVERYTHING);

    // Set attributes.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create window.
    window_ = SDL_CreateWindow(
            window_title_.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screen_width_,
            screen_height,
            SDL_WINDOW_OPENGL);  //| SDl_WINDOW_FULLSCREEN

    SDL_Rect screen;
    SDL_GetDisplayBounds(0, &screen);
    max_screen_width_ = screen.w;
    max_screen_height_ = screen.h;

    // Hide cursor and move to center of the screen.
    SDL_ShowCursor(SDL_DISABLE);
    center_cursor();

    // Create OpenGL context.
    context_ = SDL_GL_CreateContext(window_);

    // Clear screen with black color.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initialize GLEW.
    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        std::cerr << "Glew failed to initialize: " << status << std::endl;
    }

    glViewport(0, 0, screen_width_, screen_height_);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPointSize(2);
}

GraphicsSystem::~GraphicsSystem() {
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
}

void GraphicsSystem::center_cursor() {
    SDL_WarpMouseInWindow(window_, screen_width_/2, screen_height_/2);
}

void GraphicsSystem::swap_buffers() {
    SDL_GL_SwapWindow(window_);
}

void GraphicsSystem::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsSystem::toggle_fullscreen() {
    if (is_fullscreen()) {
        SDL_SetWindowFullscreen(window_, 0);
        glViewport(0, 0, screen_width_, screen_height_);
    } else {
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
        int width = (max_screen_height_ * screen_width_) / screen_height_;
        int offset = (max_screen_width_ - width) / 2;
        glViewport(offset, 0, width, max_screen_height_);
    }
}

bool GraphicsSystem::is_fullscreen() {
    int flags = SDL_GetWindowFlags(window_);
    return flags & SDL_WINDOW_FULLSCREEN;
}

void GraphicsSystem::print_fps(float fps) {
    std::string result = window_title_ + " (" +
        utils::to_string_with_precision(fps, 4) + " FPS).";
    SDL_SetWindowTitle(window_, result.c_str());
}

void GraphicsSystem::load(ShaderProgram& shader_program, Mesh& mesh) {
    // Create vertex array object.
    glGenVertexArrays(1, mesh.get_vao());
    glBindVertexArray(*mesh.get_vao());

    // Create vertex buffer object.
    glGenBuffers(1, mesh.get_vertex_buffer());
    glBindBuffer(GL_ARRAY_BUFFER, *mesh.get_vertex_buffer());
    glBufferData(
            GL_ARRAY_BUFFER,
            3*mesh.get_n_vertices() * sizeof(GLfloat),
            mesh.get_vertices(),
            GL_STATIC_DRAW);
    GLuint position_id = glGetAttribLocation(shader_program.get_program(), "position");
    glEnableVertexAttribArray(position_id);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_TRUE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create normal buffer object
    glGenBuffers(1, mesh.get_normal_buffer());
    glBindBuffer(GL_ARRAY_BUFFER, *mesh.get_normal_buffer());
    glBufferData(
        GL_ARRAY_BUFFER,
        3*mesh.get_n_vertices() * sizeof(GLfloat),
        mesh.get_normals(),
        GL_STATIC_DRAW);
    GLuint normal_id = glGetAttribLocation(shader_program.get_program(), "normal");
    glEnableVertexAttribArray(normal_id);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_TRUE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create element buffer array
    glGenBuffers(1, mesh.get_element_buffer());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mesh.get_element_buffer());
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.get_n_elements()*sizeof(GLuint),
            mesh.get_elements(),
            GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void GraphicsSystem::render(ShaderProgram& shader_program, Mesh& mesh) {
    shader_program.bind();

    glm::mat4 model_matrix_ = glm::mat4(1.0f);
    glm::mat3 normal_matrix_ = glm::mat3(1.0f);
    glm::mat4 transformation_matrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    model_matrix_ = transformation_matrix_ * model_matrix_;
    normal_matrix_ = glm::transpose(glm::inverse(glm::mat3(model_matrix_)));

    glBindVertexArray(*mesh.get_vao());

    glUniformMatrix4fv(
            glGetUniformLocation(shader_program.get_program(), "model_matrix"),
            1,
            GL_FALSE,
            glm::value_ptr(model_matrix_));  // &model_matrix[0][0] works too

    // Get normal matrix.
    glUniformMatrix3fv(
            glGetUniformLocation(shader_program.get_program(), "normal_matrix"),
            1,
            GL_FALSE,
            glm::value_ptr(normal_matrix_));

    // Use lighting.
    glUniform1i(
            glGetUniformLocation(shader_program.get_program(), "use_lighting"),
            GL_TRUE);

    // Set ambient and directional light colors.
    glUniform3f(
            glGetUniformLocation(shader_program.get_program(), "ambient_color"),
            0.2f, 0.2f, 0.2f);

    glUniform3f(
            glGetUniformLocation(shader_program.get_program(), "directional_color"),
            0.6f, 0.6f, 0.6f);

    // Set lighting direction.
    glm::vec3 adjusted_lighting_direction = glm::vec3(1, 1, 1);
    glUniform3fv(
            glGetUniformLocation(shader_program.get_program(), "lighting_direction"),
            1,
            glm::value_ptr(adjusted_lighting_direction));

    glDrawElements(GL_TRIANGLES, mesh.get_n_elements(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    //  glUseProgram(0);  // For some reason, this won't let me move the camera.
}
