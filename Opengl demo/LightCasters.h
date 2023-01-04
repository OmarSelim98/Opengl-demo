#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

struct Light {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float diffuse = 0.8f;
	float specular = 1.0f;
	float color[3] = { 1.0f,1.0f,1.0f };

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return getDiffuseVector() * 0.5f;
	}
};

struct DirectionalLight {
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
	float diffuse = 0.8f;
	float specular = 1.0f;
	float color[3] = { 1.0f,1.0f,1.0f };

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return getDiffuseVector() * 0.2f;
	}
	glm::vec3 getSpecularVector() {
		return glm::vec3(specular);
	}
};

struct PointLight {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float diffuse = 0.8f;
	float specular = 1.0f;
	float color[3] = { 1.0f,0.1f,0.1f };

	float constant = 1.0;
	float linear = 0.09f;
	float quadratic = 0.032;

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return getDiffuseVector() * 0.2f;
	}
	glm::vec3 getSpecularVector() {
		return glm::vec3(specular);
	}
};

struct SpotLight {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	float diffuse = 0.9f;
	float specular = 1.0f;
	float innerCutoff = glm::cos(glm::radians(11.0f));
	float outerCutoff = glm::cos(glm::radians(14.0f));
	float constant = 1.0;
	float linear = 0.045;
	float quadratic = 0.0075;
	float color[3] = { 1.0f,1.0f,1.0f };

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return getDiffuseVector() * 0.2f;
	}
	glm::vec3 getSpecularVector() {
		return glm::vec3(specular);
	}
};