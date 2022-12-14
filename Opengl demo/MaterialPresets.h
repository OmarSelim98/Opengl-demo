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

