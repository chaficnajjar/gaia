// Copyright [2015] <Chafic Najjar>

#include "src/mesh.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

constexpr const GLfloat Mesh::ICOSAHEDRON_VERTICES[];
constexpr const GLuint Mesh::ICOSAHEDRON_INDICES[];

Mesh::Mesh(float radius) :
    radius_(radius),
    loaded_(false) {
    // Compute the total number of vertices and faces.
    n_faces_ = N_ICOSAHEDRON_FACES;
    n_vertices_ = N_ICOSAHEDRON_VERTICES;
    for (int i = 0; i < RECURSION_LEVEL; i++) {
        n_vertices_ += n_faces_*N_TRIANGLE_VERTICES/N_FACES_SHARING_ONE_EDGE;
        n_faces_ *= N_FACES_IN_SUBDIVIDED_TRIANGLE;
    }

    // Compute vertices and normals
    vertex_count_ = 0;
    vertices_ = new GLfloat[n_vertices_*N_DIMENSIONS];
    normals_ = new GLfloat[n_vertices_*N_DIMENSIONS];
    for (int i = 0; i < N_ICOSAHEDRON_VERTICES*N_DIMENSIONS; i += N_DIMENSIONS) {
        GLfloat x = ICOSAHEDRON_VERTICES[i];
        GLfloat y = ICOSAHEDRON_VERTICES[i+1];
        GLfloat z = ICOSAHEDRON_VERTICES[i+2];
        add_vertex(x, y, z);

        glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

        normals_[i] = normal[0];
        normals_[i+1] = normal[1];
        normals_[i+2] = normal[2];
    }

    // Compute the total number of indices.
    n_indices_ = N_ICOSAHEDRON_FACES*N_TRIANGLE_VERTICES;

    // Compute indices.
    indices_ = new GLuint[n_indices_];
    for (int i = 0; i < n_indices_; i++) {
        indices_[i] = ICOSAHEDRON_INDICES[i];
    }
    for (int i = 1; i < RECURSION_LEVEL; i++) {
        GLuint* indices = new GLuint[n_indices_*N_FACES_IN_SUBDIVIDED_TRIANGLE];
        for (int j = 0, k = 0; j < n_indices_; j += N_TRIANGLE_VERTICES, k += N_SUBDIVIDED_TRIANGLE_VERTICES) {
            GLuint a = get_middle_point(indices_[j], indices_[j+1]);
            GLuint b = get_middle_point(indices_[j+1], indices_[j+2]);
            GLuint c = get_middle_point(indices_[j+2], indices_[j]);

            // First triangle.
            indices[k] = indices_[j];
            indices[k+1] = a;
            indices[k+2] = c;

            // Second triangle.
            indices[k+3] = indices_[j+1];
            indices[k+4] = b;
            indices[k+5] = a;

            // Third triangle.
            indices[k+6] = indices_[j+2];
            indices[k+7] = c;
            indices[k+8] = b;

            // Fourth triangle.
            indices[k+9] = a;
            indices[k+10] = b;
            indices[k+11] = c;
        }
        n_indices_ *= N_FACES_IN_SUBDIVIDED_TRIANGLE;
        indices_ = indices;
    }
}


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteBuffers(1, &normal_buffer_);
    glDeleteBuffers(1, &element_buffer_);
    delete indices_;
    delete normals_;
    delete vertices_;
}


GLuint Mesh::add_vertex(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat length = sqrt(x*x + y*y + z*z);

    x /= length;
    y /= length;
    z /= length;

    vertices_[N_DIMENSIONS*vertex_count_] = x * radius_;
    vertices_[N_DIMENSIONS*vertex_count_+1] = y * radius_;
    vertices_[N_DIMENSIONS*vertex_count_+2] = z * radius_;

    normals_[N_DIMENSIONS*vertex_count_] = x;
    normals_[N_DIMENSIONS*vertex_count_+1] = y;
    normals_[N_DIMENSIONS*vertex_count_+2] = z;

    return vertex_count_++;
}


float Mesh::get_radius() const {
    return radius_;
}


GLuint* Mesh::get_vao() {
    return &vao_;
}


GLuint* Mesh::get_vertex_buffer() {
    return &vertex_buffer_;
}


GLuint* Mesh::get_normal_buffer() {
    return &normal_buffer_;
}


GLuint* Mesh::get_element_buffer() {
    return &element_buffer_;
}


GLfloat* Mesh::get_vertices() const {
    return vertices_;
}


GLfloat* Mesh::get_normals() const {
    return normals_;
}


GLuint* Mesh::get_indices() const {
    return indices_;
}


bool Mesh::is_loaded() {
    if (loaded_ == false) {
        loaded_ = true;
        return false;
    } else {
        return true;
    }
}


int Mesh::get_n_indices() const {
    return n_indices_;
}


int Mesh::get_n_vertices() const {
    return n_vertices_;
}


void Mesh::set_n_vertices(int n_vertices) {
    n_vertices_ = n_vertices;
}


void Mesh::set_n_indices(int n_indices) {
    n_indices_ = n_indices;
}


GLuint Mesh::get_middle_point(GLuint p1, GLuint p2) {
    // First check if we have it already.
    GLuint smaller_index, greater_index;
    if (p1 < p2) {
        smaller_index = p1;
        greater_index = p2;
    } else {
        smaller_index = p2;
        greater_index = p1;
    }

    GLuint64 key = (GLuint64)smaller_index << 32 | greater_index;

    try {
        return middle_point_index_cache.at(key);
    } catch (const std::out_of_range& error) {}

    // Not in cache, calculate it.
    glm::vec3 point1 = glm::vec3(vertices_[N_DIMENSIONS*p1], vertices_[N_DIMENSIONS*p1+1], vertices_[N_DIMENSIONS*p1+2]);
    glm::vec3 point2 = glm::vec3(vertices_[N_DIMENSIONS*p2], vertices_[N_DIMENSIONS*p2+1], vertices_[N_DIMENSIONS*p2+2]);
    glm::vec3 middle_point = glm::vec3( (point1[0]+point2[0])/2, (point1[1]+point2[1])/2, (point1[2]+point2[2])/2 );

    GLuint i = add_vertex(middle_point[0], middle_point[1], middle_point[2]);

    middle_point_index_cache[key] = i;

    return i;
}
