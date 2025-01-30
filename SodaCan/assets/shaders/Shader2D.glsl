@vertex
#version 410 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_texScale;

out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;
out float v_texScale;

uniform mat4 u_PVMat;

void main()
{
  gl_Position = u_PVMat * a_position;

  v_texCoord = a_texCoord;
  v_color = a_color;
  v_texIndex = a_texIndex;
  v_texScale = a_texScale;
}

@fragment
#version 410 core

layout(location = 0) out vec4 Color;

in vec2 v_texCoord;
in vec4 v_color;
in float v_texIndex;
in float v_texScale;

uniform sampler2D u_Textures[32];

void main()
{
  Color = texture(u_Textures[v_texIndex], v_texCoord * v_texScale) * v_color;
}