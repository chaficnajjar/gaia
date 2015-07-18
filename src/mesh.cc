// Copyright [2015] <Chafic Najjar>

#include "src/mesh.h"

Mesh::Mesh(float radius, int n_latitude_bands, int n_longitude_bands) :
    radius_(radius),
    n_latitude_bands_(n_latitude_bands),
    n_longitude_bands_(n_longitude_bands),
    loaded_(false) {
    const float PI = 3.14159f;

    n_vertices_ = (n_latitude_bands_+1) * (n_longitude_bands_+1);
    n_elements_ = 6 * n_latitude_bands_ * n_longitude_bands_;

    vertices_ = new GLfloat[3 * n_vertices_];
    normals_ = new GLfloat[3 * n_vertices_];
    elements_ = new GLuint[n_elements_];
    int index = 0;

    std::cout << "Number of vertices: " << n_vertices_ << std::endl;
    std::cout << "Number of normals: " << n_vertices_ << std::endl;
    std::cout << "Number of elements: " << n_elements_ << std::endl;
    for (int i = 0; i <= n_latitude_bands_; i++) {
        float theta = i * PI / n_latitude_bands_;
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);
        float y = cos_theta;
        for (int j = 0; j <= n_longitude_bands_; j++) {
            float phi = static_cast<float>(j) * 2.0 * PI / static_cast<float>(n_longitude_bands_);
            float sin_phi = std::sin(phi);
            float cos_phi = std::cos(phi);

            float x = cos_phi * sin_theta;
            float z = sin_phi * sin_theta;

            normals_[index] = x;
            vertices_[index++] = radius_ * x;
            normals_[index] = y;
            vertices_[index++] = radius_ * y;
            normals_[index] = z;
            vertices_[index++] = radius_ * z;
        }
    }

    index = 0;
    for (int i = 0; i < n_latitude_bands_; i++) {
        for (int j = 0; j < n_longitude_bands_; j++) {
            int first = i * (n_longitude_bands_+1) + j;
            int second = first + n_longitude_bands_ + 1;

            elements_[index++] = first;
            elements_[index++] = second;
            elements_[index++] = first + 1;

            elements_[index++] = second;
            elements_[index++] = second + 1;
            elements_[index++] = first + 1;
        }
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteBuffers(1, &normal_buffer_);
    glDeleteBuffers(1, &element_buffer_);
    delete elements_;
    delete normals_;
    delete vertices_;
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


int Mesh::get_n_latitude_bands() const {
    return n_latitude_bands_;
}


int Mesh::get_n_longitude_bands() const {
    return n_longitude_bands_;
}


GLfloat* Mesh::get_vertices() const {
    return vertices_;
}


GLfloat* Mesh::get_normals() const {
    return normals_;
}


GLuint* Mesh::get_elements() const {
    return elements_;
}


bool Mesh::is_loaded() {
    if (loaded_ == false) {
        loaded_ = true;
        return false;
    } else {
        return true;
    }
}


int Mesh::get_n_elements() const {
    return n_elements_;
}


int Mesh::get_n_vertices() const {
    return n_vertices_;
}


void Mesh::set_n_vertices(int n_vertices) {
    n_vertices_ = n_vertices;
}

void Mesh::set_n_elements(int n_elements) {
    n_elements_ = n_elements;
}
