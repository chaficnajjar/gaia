// Copyright [2015] <Chafic Najjar>

#include "src/engine.h"

Engine::Engine() :
    exit(false),
    graphics_system_(config.get_width(), config.get_height()),
    input_system_(config.get_width(), config.get_height()),
    mesh_(1, 30, 30),
    shader_program_("src/shader.vert", "src/shader.frag"),
    camera_(shader_program_, config.get_width(), config.get_height()) {
    graphics_system_.load(shader_program_, mesh_);
}

Engine::~Engine() {
    SDL_Quit();
}

void Engine::execute() {
    int start_time = SDL_GetTicks();
    int n_frames = 0;
    int last_update_time = SDL_GetTicks();

    while (!exit) {
        input();

        const int current_time_ms = SDL_GetTicks();
        update(current_time_ms - last_update_time);
        last_update_time = current_time_ms;

        render();

        int elapsed_time_ms = SDL_GetTicks() - start_time;
        double elapsed_time_s = elapsed_time_ms / 1000.0;
        double fps = ++n_frames / elapsed_time_s;

        if (n_frames % 20 == 0) {
            graphics_system_.print_fps(fps);
        }
    }
}

void Engine::input() {
    exit = input_system_.handle_input();
    graphics_system_.center_cursor();

    if (exit && graphics_system_.is_fullscreen()) {
        graphics_system_.toggle_fullscreen();
    } else if (input_system_.fullscreen_toggled()) {
        graphics_system_.toggle_fullscreen();
    }
}

void Engine::update(int elapsed_time_ms) {
    camera_.update(input_system_);
}

void Engine::render() {
    graphics_system_.clear();
    graphics_system_.render(shader_program_, mesh_);
    graphics_system_.swap_buffers();
}
