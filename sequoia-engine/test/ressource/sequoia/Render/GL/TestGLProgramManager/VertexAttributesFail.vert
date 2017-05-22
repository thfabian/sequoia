#version 330 core

in vec3 in_PositionInvalid;

void main() {
  gl_Position = vec4(in_PositionInvalid.xyz, 1.0);
}

