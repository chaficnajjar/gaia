VERSION

#include SHADER

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
out vec3 color;

void main() {
    float n = snoise(position * 10.0)/25;
    n += 0.5 * snoise(position * 20.0)/25;
    n += 0.25 * snoise(position * 40.0)/25;
    n += 0.125 * snoise(position * 80.0)/25;
    float a = n - 0.05;
    /*if (n > 0.7/10) {*/
        /*color = vec3(0.647, 0.949, 0.953);*/
    /*} else {*/
    color = vec3(0.498, 0.322, 0.09) + vec3(a, a, a);
    /*}*/
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position*(n+1), 1.0);

    if (use_lighting) {
        vec3 transformed_normal = normal_matrix * normal;
        float directional_light_weighting = max(dot(transformed_normal, lighting_direction), 0.0);
        lighting_weighting = ambient_color + directional_color * directional_light_weighting;
    } else {
        lighting_weighting = vec3(0.0, 0.0, 0.0);
    }
}
