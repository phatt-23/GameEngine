#type vertex
#version 410

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;
uniform vec4 u_Color;

out vec4 v_Color;

void main(void)
{
    v_Color = u_Color;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type pixel
#version 410

in vec4 v_Color;

out vec4 f_Color;

void main(void)
{
    f_Color = v_Color;
}
