#shader vertex
#version 330 core
        
layout(location = 0) in vec3 pos3;
layout(location = 1) in vec3 norm3;
layout(location = 3) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 normal;
uniform mat4 u_MVP;

void main()
{
    vec4 position = vec4(pos3, 1);
    normal = vec4(norm3, 1);
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
};



#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 normal;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor + u_Color;
    color = vec4(1, 1, 1, 1);
    color = normal;
};