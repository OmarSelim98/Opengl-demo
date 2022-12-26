#version 330 core

struct Material{
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D diffuse3;
    sampler2D specular1;
    sampler2D specular2;
    sampler2D specular3;
    sampler2D emission;
    float shininess;
};
struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
struct SpotLight{
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    sampler2D cookie;
};


vec3 calculateDirectionalLight(DirLight dirLight, Material mat, vec3 normal, vec3 viewDir);


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat4 ScreenSpaceMat;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;


void main()
{
    
    vec3 result = vec3(0.0);
    result   += calculateDirectionalLight(dirLight, material, Normal, normalize(viewPos - FragPos));
    result   += calculateDirectionalLight(dirLight, material, Normal, normalize(viewPos - FragPos));

    //result   += calculatePointLight(pointLight, material, Normal, normalize(viewPos - FragPos));
   // result   += calculateSpotLight(spotLight, material);
    FragColor = vec4(result, 1.0);
}

vec3 calculateDirectionalLight(DirLight dirLight, Material mat, vec3 normal, vec3 viewDir){
    
    //diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diffFactor = max(dot(normalize(normal), lightDir) , 0);

    vec3 diffuse = diffFactor * dirLight.diffuse * vec3(texture(mat.diffuse1, TexCoords)) * vec3(texture(mat.diffuse2, TexCoords)) * vec3(texture(mat.diffuse3, TexCoords));

    //ambient
    vec3 ambient = dirLight.ambient * vec3(texture(mat.diffuse1, TexCoords)) * vec3(texture(mat.diffuse2, TexCoords)) * vec3(texture(mat.diffuse3, TexCoords));

    //specular
    vec3 reflection = reflect(-lightDir, normalize(normal));
    float specFactor = pow(max(dot(reflection,viewDir),0),mat.shininess);

    vec3 specular = specFactor * dirLight.specular * vec3(texture(mat.specular1, TexCoords)) * vec3(texture(mat.specular2, TexCoords)) * vec3(texture(mat.specular3, TexCoords));

    return ambient + diffuse + specular;
}
