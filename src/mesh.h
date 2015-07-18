// Copyright [2015] <Chafic Najjar>

#ifndef SRC_MESH_H_
#define SRC_MESH_H_

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <cmath>
#include <iostream>

class Mesh {
 public:
     Mesh(float radius, int n_latitude_bands, int n_longitude_bands);
     ~Mesh();

     GLfloat* get_vertices() const;
     GLfloat* get_normals() const;
     GLuint* get_elements() const;

     bool is_loaded();

     int get_n_vertices() const;
     int get_n_elements() const;
     void set_n_vertices(int n_vertices);
     void set_n_elements(int n_elements);
     int get_n_latitude_bands() const;
     int get_n_longitude_bands() const;
     GLuint* get_vao();
     GLuint* get_vertex_buffer();
     GLuint* get_normal_buffer();
     GLuint* get_element_buffer();
     float get_radius() const;

 private:
     float radius_;
     int n_latitude_bands_, n_longitude_bands_;
     GLfloat* vertices_;
     GLfloat* normals_;
     GLuint* elements_;
     int n_elements_, n_vertices_;
     bool loaded_;
     GLuint vao_, vertex_buffer_, normal_buffer_, element_buffer_;
};

#endif  // SRC_MESH_H_

