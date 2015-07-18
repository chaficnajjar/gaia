// Copyright [2015] <Chafic Najjar>

#ifndef SRC_GRAPHICS_SYSTEM_H_
#define SRC_GRAPHICS_SYSTEM_H_

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <string>

#include "src/utils.h"

class ShaderProgram;
class Mesh;

class GraphicsSystem {
 public:
    GraphicsSystem(int screen_width, int screen_height);
    ~GraphicsSystem();
    void center_cursor();
    void swap_buffers();
    void clear();
    void toggle_fullscreen();
    bool is_fullscreen();
    void print_fps(float fps);
    void load(ShaderProgram& shader_program, Mesh& mesh);
    void render(ShaderProgram& shader_program, Mesh& mesh);

 private:
    std::string window_title_;
    int screen_width_, screen_height_;
    int max_screen_width_, max_screen_height_;
    SDL_Window* window_;
    SDL_GLContext context_;
};


#endif  // SRC_GRAPHICS_SYSTEM_H_
