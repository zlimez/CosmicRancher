#version 330 core
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse;
// uniform sampler2D texture_specular;

void main() {
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    FragColor = texture(texture_diffuse, TexCoords);
}