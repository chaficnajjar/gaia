#version 130

in vec3 lighting_weighting;

void main() {
    vec3 red = vec3(1.0, 0.0, 0.0);
    gl_FragColor = vec4(red * lighting_weighting, 1.0);
}
