#shader vertex
#version 330 core
        
layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 norm;
layout(location = 3) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 normal;
out vec4 LightDir;
out vec4 position;

uniform mat4 u_MVP;
uniform vec3 u_LightPos; 

void main()
{
    vec4 lightPos = vec4(u_LightPos, 1);
    vec4 position = pos;
    normal = norm;
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;

    LightDir = normalize(lightPos - gl_Position);
};



#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 normal;
in vec4 LightDir;
in vec4 position;


uniform vec4 u_Color;
uniform vec4 u_LightColor;
uniform vec3 u_ViewPos;
uniform float u_AmbientIntensity;
uniform float u_SpecularIntensity;
uniform sampler2D u_Texture;

void main()
{
    //color = texColor + u_Color;
    //color = vec4(normal.r, normal.g, normal.b, 0.0);
    //color = color + u_Color;

    //Ambient
    //color = u_Color;
    //vec4 ambient = vec4(color.r * u_AmbientIntensity, color.g * u_AmbientIntensity, color.b * u_AmbientIntensity, color.a);
    vec4 ambient = u_AmbientIntensity * u_LightColor;

    //diffuse
    float diff = max(dot(normalize(normal), normalize(LightDir)), 0.0);
    vec4 diffuse = diff * u_LightColor;


    //spec
    vec4 viewPos = vec4(u_ViewPos, 1);
    vec4 viewDir = normalize(viewPos - position);
    vec4 reflectDir = reflect(-LightDir, normalize(normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularIntensity);
    vec4 specular = spec * u_LightColor;
    
    //phong
    color = (ambient + diffuse) + specular;
    color = vec4(color.r, color.g, color.b, 1);


    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;


};