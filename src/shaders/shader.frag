#version 130

in vec3 lighting_weighting;
in vec3 color;

void main() {
    gl_FragColor = vec4(color * lighting_weighting, 1.0);
}
