#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "shader_class.h"

struct Light {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float diffuse = 0.8f;
	float specular = 1.0f;
	float color[3] = { 1.0f,1.0f,1.0f};

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return getDiffuseVector() * 0.5f;
	}
};

struct DirectionalLight {
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 1.0f);
	float diffuse = 0.8f;
	float specular = 1.0f;
	float color[3] = { 1.0f,0.0f,0.0f };

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
	void ApplyIn(std::string objectName ,Shader& shader) {
	
		shader.setVec3(objectName + ".direction", direction);
		shader.setVec3(objectName + ".ambient", getAmbientVector());
		shader.setVec3(objectName + ".diffuse", getDiffuseVector());
		shader.setVec3(objectName + ".specular", getSpecularVector());
	}
};

struct PointLight {
	glm::vec3 position = glm::vec3(0.5f, 1.0f, 0.3f);
	float diffuse = 0.8f;
	float specular = 1.0f;
	float color[3] = { 1.0f,1.0f,1.0f };

	float constant = 1.0f;
	float linear = 0.22;
	float quadratic = 0.20;

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return glm::make_vec3(color) * 0.5f;
	}
	glm::vec3 getSpecularVector() {
		return glm::vec3(specular);
	}
	void ApplyIn(std::string objectName, Shader& shader) {
		shader.setVec3(objectName + ".position", position);
		shader.setVec3(objectName + ".ambient", getAmbientVector());
		shader.setVec3(objectName + ".diffuse", getDiffuseVector());
		shader.setVec3(objectName + ".specular", getSpecularVector());
		shader.setFloat(objectName + ".constant", constant);
		shader.setFloat(objectName + ".linear",linear);
		shader.setFloat(objectName + ".quadratic", quadratic);
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