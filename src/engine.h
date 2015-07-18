// Copyright [2015] <Chafic Najjar>

#ifndef SRC_ENGINE_H_
#define SRC_ENGINE_H_

#include "src/camera.h"
#include "src/config.h"
#include "src/graphics_system.h"
#include "src/input_system.h"
#include "src/mesh.h"
#include "src/shader_program.h"

class Engine {
 public:
    Engine();
    ~Engine();
    void execute();

 private:
    bool exit;
    Config config;
    GraphicsSystem graphics_system_;
    InputSystem input_system_;
    Mesh mesh_;
    ShaderProgram shader_program_;
    Camera camera_;

    void input();
    void update(int elapsed_time);
    void render();
};

#endif  // SRC_ENGINE_H_
