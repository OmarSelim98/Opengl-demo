#pragma once
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model {
public:
	Model(std::string path);
	void Destroy();
	inline std::vector<Mesh> Meshes() const{ return m_Meshes; };
private:
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	std::vector<Tex> textures_loaded;

	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
	unsigned int TextureFromFile(std::string fileName, std::string directory);
};