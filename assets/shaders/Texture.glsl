#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

// uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;

void main() 
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    // gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TilingFactor;

out vec4 f_Color;

void main() 
{
    // f_Color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
    f_Color = v_Color;
}

