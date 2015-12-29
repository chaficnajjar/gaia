// Copyright [2015] <Chafic Najjar>

#ifndef SRC_MESH_H_
#define SRC_MESH_H_

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <cmath>
#include <iostream>
#include <unordered_map>

class Mesh {
 public:
    Mesh(float radius);
    ~Mesh();

    GLfloat* get_vertices() const;
    GLfloat* get_normals() const;
    GLuint* get_indices() const;

    bool is_loaded();

    int get_n_vertices() const;
    int get_n_indices() const;
    void set_n_vertices(int n_vertices);
    void set_n_indices(int n_indices);
    GLuint* get_vao();
    GLuint* get_vertex_buffer();
    GLuint* get_normal_buffer();
    GLuint* get_element_buffer();
    float get_radius() const;

 private:
    static const int N_DIMENSIONS = 3;
    static const int N_TRIANGLE_VERTICES = 3;
    static const int N_FACES_SHARING_ONE_EDGE = 2;

    static const int RECURSION_LEVEL = 4;
    static const int N_FACES_IN_SUBDIVIDED_TRIANGLE = 4;
    static const int N_SUBDIVIDED_TRIANGLE_VERTICES = 12;

    static const int N_ICOSAHEDRON_VERTICES = 12;
    static const int N_ICOSAHEDRON_FACES = 20;
    static constexpr float t = (1.0 + sqrt(5.0)) / 2.0;  // Golden ratio.
    static constexpr GLfloat ICOSAHEDRON_VERTICES[] = {
        -1,  t,  0,
         1,  t,  0,
        -1, -t,  0,
         1, -t,  0,

         0, -1,  t,
         0,  1,  t,
         0, -1, -t,
         0,  1, -t,

         t,  0, -1,
         t,  0,  1,
        -t,  0, -1,
        -t,  0,  1
    };
    static constexpr GLuint ICOSAHEDRON_INDICES[] = {
        // 5 faces around point 0
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,

        // 5 adjacent faces
        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,

        // 5 faces around point 3
        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        // 5 adjacent faces
        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1
    };

    float radius_;
    bool loaded_;

    int vertex_count_;
    int n_faces_, n_indices_, n_vertices_;
    GLfloat *vertices_, *normals_;
    GLuint* indices_;
    GLuint vao_, vertex_buffer_, normal_buffer_, element_buffer_;
    std::unordered_map<GLuint64, GLuint> middle_point_index_cache;

    GLuint add_vertex(GLfloat x, GLfloat y, GLfloat z);
    GLuint get_middle_point(GLuint p1, GLuint p2);
};

#endif  // SRC_MESH_H_

