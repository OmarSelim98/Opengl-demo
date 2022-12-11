#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>

struct MaterialPreset {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};
struct Light {
	glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
	float diffuse = 0.5f;
	float specular = 1.0f;
	float color[3] = {1.0f,1.0f,1.0f};

	glm::vec3 getDiffuseVector()
	{
		return glm::vec3(diffuse) * glm::make_vec3(color);
	}
	glm::vec3 getAmbientVector() {
		return getDiffuseVector() * 0.2f;
	}
};


std::vector<MaterialPreset> ImportMaterials() {
	std::vector<MaterialPreset> presets;

	std::string name;
	float ar, ab, ag, dr, db, dg, sr, sb, sg, sh;

	std::ifstream infile("materials_presets.mp");

	while (infile >> name >> ar >> ab >> ag >> dr >> db >> dg >> sr >> sb >> sg >> sh) {
		presets.push_back({ name , glm::vec3(ar,ab,ag), glm::vec3(dr,db,dg), glm::vec3(sr,sb,sg), sh * 128.0f });
	}
	return presets;
}

//const MaterialPreset PRESETS[1] = {};

