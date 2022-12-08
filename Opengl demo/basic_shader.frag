#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientIntensity = 0.25;
vec3 ambient = ambientIntensity * lightColor;

float specularIntensity = 0.5;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    //calculate diffusion
    float diffFactor = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diffFactor * lightColor;
    //calculate specular
    vec3 veiwDir = normalize(viewPos - FragPos);
    vec3 reflection = reflect(-lightDir, norm); // reflect the direction from light source to the fragment around the normal.

    float specFactor = pow(max(dot(veiwDir, reflection), 0.0), 32);
    vec3 specular = specFactor * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}