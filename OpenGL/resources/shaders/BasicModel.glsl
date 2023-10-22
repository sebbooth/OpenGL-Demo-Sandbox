#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 v_Normal;
out vec3 v_LightDir;
out vec3 v_ViewDir;

uniform mat4 u_MVP;
uniform mat4 u_Mod;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_Normal = u_Mod*normal;
    vec4 w_Position = u_Mod * position;
    v_LightDir = u_LightPos - vec3(w_Position);
    v_ViewDir = u_ViewPos - vec3(w_Position);
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Normal;
in vec3 v_LightDir;
in vec3 v_ViewDir;

uniform vec4 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;
uniform mat4 u_Mod;

uniform float u_SpecularIntensity;

uniform float u_AmbientIntensity;
uniform sampler2D u_Texture;

void main()
{
    //texture color
    vec4 texColor = texture(u_Texture, v_TexCoord);

    //ambient light
    vec3 ambient = u_LightColor * u_AmbientIntensity;

    // Diffuse
    float diff = max(dot(normalize(vec3(v_Normal)), normalize(v_LightDir)), 0.0);
    vec3 diffuse = diff * u_LightColor;


    // Specular
    vec3 reflectDir = reflect(v_LightDir, normalize(vec3(v_Normal)));
    float spec = pow(max(dot(v_ViewDir, reflectDir), 0.0), u_SpecularIntensity);
    vec3 specular = spec * u_LightColor * 0.001;


    vec4 lighting = vec4(ambient + diffuse + specular, 1);

    color = texColor * u_Color * lighting;
};