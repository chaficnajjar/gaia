#version 130

in vec3 position;
in vec3 normal;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

uniform bool use_lighting;
uniform vec3 ambient_color;
uniform vec3 directional_color;
uniform vec3 lighting_direction;

out vec3 lighting_weighting;

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);

    if (use_lighting) {
        vec3 transformed_normal = normal_matrix * normal;
        float directional_light_weighting = max(dot(transformed_normal, lighting_direction), 0.0);
        lighting_weighting = ambient_color + directional_color * directional_light_weighting;
    } else {
        lighting_weighting = vec3(0.0, 0.0, 0.0);
    }
}
