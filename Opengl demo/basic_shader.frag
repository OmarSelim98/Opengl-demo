#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
    
    //calculate diffusion
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);

    float diffFactor = max(dot(norm, lightDir),0.0);
    vec3 diffuse = light.diffuse * diffFactor * vec3(texture(material.diffuse, TexCoords));//;vec3(texture(material.diffuse,TexCoords));

    //calculate ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));//vec3(texture(material.diffuse, TexCoords));

    //calculate specular
    vec3 veiwDir = normalize(viewPos - FragPos);
    vec3 reflection = reflect(-lightDir, norm); // reflect the direction from light source to the fragment around the normal.

    float specFactor = pow(max(dot(veiwDir, reflection), 0.0), material.shininess);
    vec3 specular = light.specular * specFactor * vec3(texture(material.specular,TexCoords));

    //calculate emission
    float emissionFactor = 1.0f;
    vec3 emission = emissionFactor * vec3(texture(material.emission,TexCoords));

    vec3 result = (ambient + diffuse + specular + emission);
    FragColor = vec4(result, 1.0);
}