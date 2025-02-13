#type    vertex// slf //??
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main() 
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

in vec2 v_TexCoord;

un
iform vec3 u_Color;
uniform sampler2D u_Texture;

out vec4 f_Color;

void main() 
{
    f_Color = texture(u_Texture, v_TexCoord);
    // f_Color = vec4(v_TexCoord, 0.0f, 1.0f) + vec4(u_Color, 0.2f);
}
