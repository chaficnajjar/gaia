// Copyright [2015] <Chafic Najjar>

#include "src/input_system.h"

InputSystem::InputSystem(int screen_width, int screen_height) :
    screen_width_(screen_width),
    screen_height_(screen_height),
    toggle_fullscreen_(false) {
    config.get_keys(key_values_, 6);

    for (int i = 0; i < 6; i++) {
        ActionNumber current_action = (ActionNumber)i;
        key_held_action_[key_values_[i]] = current_action;
    }
}

InputSystem::~InputSystem() {}

bool InputSystem::handle_input() {
    clear_key_presses();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return true;
        } else if (event.type == SDL_KEYDOWN) {
            key_down(event);
        } else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return true;
            } else {
                key_up(event);
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouse_x_, &mouse_y_);
            delta_x_ = screen_width_/2 - mouse_x_;
            delta_y_ = screen_height_/2 - mouse_y_;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Do nothing for now.
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            // Do nothing for now.
        }
    }

    if (action_map_[SDLK_F11] == EXECUTE) {
        toggle_fullscreen_ = !toggle_fullscreen_;
    }

    // Add corresponding action when key is pressed.
    for (auto it = key_pressed_action_.begin();
            it != key_pressed_action_.end(); it++) {
        if (action_map_[it->first]) {
            actions_.push(it->second);
        }
    }

    // Add corresponding action when key is held.
    for (auto it = key_held_action_.begin(); it != key_held_action_.end(); it++) {
        if (state_map_[it->first]) {
            actions_.push(it->second);
        }
    }

    return false;
}

State InputSystem::get_key_state(SDL_Keycode key) {
    return state_map_[key];
}

Action InputSystem::get_key_action(SDL_Keycode key) {
    return action_map_[key];
}

void InputSystem::clear_key_presses() {
    action_map_.clear();
}

void InputSystem::key_down(const SDL_Event& event) {
    if (state_map_[event.key.keysym.sym] == RELEASED) {
        action_map_[event.key.keysym.sym] = EXECUTE;
    }
    state_map_[event.key.keysym.sym] = PRESSED;
}

void InputSystem::key_up(const SDL_Event& event) {
    state_map_[event.key.keysym.sym] = RELEASED;
}

bool InputSystem::is_held(SDL_Keycode key) {
    return state_map_[key];
}

bool InputSystem::was_pressed(SDL_Keycode key) {
    return action_map_[key];  // || range_map[key];
}

int InputSystem::get_delta_x() {
    return delta_x_;
}

int InputSystem::get_delta_y() {
    return delta_y_;
}

bool InputSystem::fullscreen_toggled() {
    if (!toggle_fullscreen_) {
        return false;
    } else {
        toggle_fullscreen_ = false;
        return true;
    }
}
