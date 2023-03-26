#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

#define PI 3.14159265359

struct PointLight{
	vec3 position;
	vec3 color;
};
uniform vec3 viewPos;
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform PointLight light[4];

vec3 Fresnel(float angle , vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

void main(){
	// MAIN DIRECTION VECTORS
	vec3 N = normalize(Normal);
	vec3 V = normalize(viewPos - WorldPos);

	vec3 L0 = vec3(0.0);
	//MAIN LIGHT VECTORS
	for (int i = 0; i < 4; i++){
		vec3 L = normalize(light[i].position - WorldPos);
		vec3 H = normalize(V+L);

		float dist = length(light[i].position - WorldPos);
		float attenuation = 1 / (dist*dist);

		vec3 radiance = light[i].color * attenuation;
		vec3 f0 = vec3(0.04); // for non metallic
		f0 = mix(f0, albedo, metallic); // mix the base color with the f0 based on the metalness of the material.
		vec3 F = Fresnel(max(dot(H, V),0.0), f0);
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);

		vec3 num = NDF * G * F;
		float den = 4 * max(dot(N,V),0.0) * max(dot(N,L), 0.0);
		vec3 specular = num / max(den, 0.0001);

		vec3 ks = F;
		vec3 kd = vec3(1.0) - F;

		kd *= (1-metallic); // metal doesn't refract light

		float NdotL = max(dot(N, L), 0.0);

		L0 += (kd * albedo / PI + specular) * radiance * NdotL;

		
	}
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + L0;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}
vec3 Fresnel(float angle , vec3 F0){
	return F0 + (1-F0) * pow(1-angle, 5);
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;
	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}