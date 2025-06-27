#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform bool selected;
uniform samplerCube skybox;

void main()
{
    if (selected) {
        FragColor = vec4(1.0, 0.0, 0.0, 0.0);
    }
    else {
        vec3 ambient = material.ambient * lightColor;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (diff * material.diffuse) * lightColor;
            
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess / 10.0);
        vec3 specular = lightColor * (spec * material.specular);

        //may remove this, supposed to check if texture is passed in. Seems to be working.
        if (textureSize(texture_diffuse1, 0).x > 0) {
            FragColor = vec4(ambient + diffuse + spec, 1.0) * texture(texture_diffuse1, texCoord);
        }
        else {
            FragColor = vec4(ambient + diffuse + spec, 1.0);
        }
    }
}