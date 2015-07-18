// Copyright [2015] <Chafic Najjar>

#ifndef SRC_INPUT_SYSTEM_H_
#define SRC_INPUT_SYSTEM_H_

#include <SDL2/SDL.h>

#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>

#include "src/config.h"

enum InputType {
    ACTION,
    STATE,
    RANGE
};

enum Action {
    EXECUTE = true,
    STOP = false
};

enum State {
    PRESSED = true,
    RELEASED = false
};

class InputSystem {
 public:
    enum ActionNumber {
        MOVE_RIGHT      = 0,
        MOVE_LEFT       = 1,
        MOVE_BACKWARD   = 2,
        MOVE_FORWARD    = 3,
        MOVE_UPWARD     = 4,
        MOVE_DOWNWARD   = 5,
        COUNT = 6
    };

    InputSystem(int screen_width, int screen_height);
    ~InputSystem();

    bool handle_input();
    State get_key_state(SDL_Keycode key);
    Action get_key_action(SDL_Keycode key);
    void clear_key_presses();

    int get_delta_x();
    int get_delta_y();

    bool fullscreen_toggled();

    std::queue<ActionNumber> actions_;

 private:
    Config config;
    int key_values_[6];

    int screen_width_, screen_height_;
    bool toggle_fullscreen_;
    int mouse_x_, mouse_y_;
    int old_mouse_x_, old_mouse_y;
    int delta_x_, delta_y_;

    void key_down(const SDL_Event& event);
    void key_up(const SDL_Event& event);

    bool is_held(SDL_Keycode key);
    bool was_pressed(SDL_Keycode key);

    std::map <SDL_Keycode, State> state_map_;
    std::map <SDL_Keycode, Action> action_map_;

    std::unordered_map <SDL_Keycode, ActionNumber> key_held_action_;
    std::unordered_map <SDL_Keycode, ActionNumber> key_pressed_action_;
};

#endif  // SRC_INPUT_SYSTEM_H_
