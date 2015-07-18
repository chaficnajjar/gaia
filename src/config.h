// Copyright [2015] <Chafic Najjar>

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#include <iostream>

class Config {
 public:
    Config();
    ~Config();

    int get_width() const;
    int get_height() const;
    void get_keys(int* key_values, int n_keys) const;

 private:
    lua_State* L;
    int get_field(const int key) const;
};

#endif  // SRC_CONFIG_H_
