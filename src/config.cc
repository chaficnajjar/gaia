// Copyright [2015] <Chafic Najjar>

#include "src/config.h"

Config::Config() {
    // Initialize lua.
    L = luaL_newstate();

    // Load I/O library.
    luaL_openlibs(L);

    // Load configuration file.
    if (luaL_loadfile(L, "src/config.lua")) {
        std::cerr << "Something went wrong loading the chunk: ";
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    // Execute configuration file.
    if (lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Something went wrong while executing: ";
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

Config::~Config() {
    lua_close(L);
}

int Config::get_width() const {
    lua_getglobal(L, "width");
    int width = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return width;
}

int Config::get_height() const {
    lua_getglobal(L, "height");
    int height = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return height;
}

void Config::get_keys(int* key_values, int n_keys) const {
    // Pushes "keys" onto the stack.
    lua_getglobal(L, "keys");

    // Checks if "keys" is a table.
    if (!lua_istable(L, -1)) {
        std::cout << "'keys' is not a valid table." << std::endl;
    }

    for (int i = 1; i <= n_keys; i++) {
        key_values[i-1] = get_field(i);
    }

    lua_pop(L, 1);
}

int Config::get_field(const int key) const {
    // Pushed the value of "key" onto the stack.
    lua_pushnumber(L, key);

    lua_gettable(L, -2);

    // Checks whether the value at the top of the stack is a number.
    if (!lua_isnumber(L, -1)) {
        std::cout << "Invalid component in background color." << std::endl;
    }

    int result = static_cast<int>(lua_tonumber(L, -1));

    lua_pop(L, 1);

    return result;
}
