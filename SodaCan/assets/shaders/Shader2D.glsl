@vertex
#version 460 core

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
#version 460 core

layout(location = 0) out vec4 Color;

in vec2 v_texCoord;
in vec4 v_color;
in float v_texIndex;
in float v_texScale;

uniform sampler2D u_Textures[32];

void main()
{
  vec4 texColor = v_color;
  switch (int(v_texIndex))
  {
    case 0:  texColor *= texture(u_Textures[0], v_texCoord * v_texScale); break;
    case 1:  texColor *= texture(u_Textures[1], v_texCoord * v_texScale); break;
    case 2:  texColor *= texture(u_Textures[2], v_texCoord * v_texScale); break;
    case 3:  texColor *= texture(u_Textures[3], v_texCoord * v_texScale); break;
    case 4:  texColor *= texture(u_Textures[4], v_texCoord * v_texScale); break;
    case 5:  texColor *= texture(u_Textures[5], v_texCoord * v_texScale); break;
    case 6:  texColor *= texture(u_Textures[6], v_texCoord * v_texScale); break;
    case 7:  texColor *= texture(u_Textures[7], v_texCoord * v_texScale); break;
    case 8:  texColor *= texture(u_Textures[8], v_texCoord * v_texScale); break;
    case 9:  texColor *= texture(u_Textures[9], v_texCoord * v_texScale); break;
    case 10: texColor *= texture(u_Textures[10], v_texCoord * v_texScale); break;
    case 11: texColor *= texture(u_Textures[11], v_texCoord * v_texScale); break;
    case 12: texColor *= texture(u_Textures[12], v_texCoord * v_texScale); break;
    case 13: texColor *= texture(u_Textures[13], v_texCoord * v_texScale); break;
    case 14: texColor *= texture(u_Textures[14], v_texCoord * v_texScale); break;
    case 15: texColor *= texture(u_Textures[15], v_texCoord * v_texScale); break;
    case 16: texColor *= texture(u_Textures[16], v_texCoord * v_texScale); break;
    case 17: texColor *= texture(u_Textures[17], v_texCoord * v_texScale); break;
    case 18: texColor *= texture(u_Textures[18], v_texCoord * v_texScale); break;
    case 19: texColor *= texture(u_Textures[19], v_texCoord * v_texScale); break;
    case 20: texColor *= texture(u_Textures[20], v_texCoord * v_texScale); break;
    case 21: texColor *= texture(u_Textures[21], v_texCoord * v_texScale); break;
    case 22: texColor *= texture(u_Textures[22], v_texCoord * v_texScale); break;
    case 23: texColor *= texture(u_Textures[23], v_texCoord * v_texScale); break;
    case 24: texColor *= texture(u_Textures[24], v_texCoord * v_texScale); break;
    case 25: texColor *= texture(u_Textures[25], v_texCoord * v_texScale); break;
    case 26: texColor *= texture(u_Textures[26], v_texCoord * v_texScale); break;
    case 27: texColor *= texture(u_Textures[27], v_texCoord * v_texScale); break;
    case 28: texColor *= texture(u_Textures[28], v_texCoord * v_texScale); break;
    case 29: texColor *= texture(u_Textures[29], v_texCoord * v_texScale); break;
    case 30: texColor *= texture(u_Textures[30], v_texCoord * v_texScale); break;
    case 31: texColor *= texture(u_Textures[31], v_texCoord * v_texScale); break;
  }
  Color = texColor;
}
