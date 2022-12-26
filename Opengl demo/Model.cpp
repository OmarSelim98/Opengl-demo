#include "Model.h"
//#include <iostream>

Model::Model(std::string path) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}
	else {
		//std::cout << "SUCCESS::ASSIMP::MODEL::LOADED" << std::endl;
		std::cout << "SUCCESS::ASSIMP::MODEL::LOADED::" << scene->HasMeshes() << std::endl;
	}

	m_Directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::Destroy()
{
	for (int i = 0; i < this->Meshes().size(); i++) {
		this->Meshes()[i].Destroy();
	}
}

void Model::processNode(const aiNode* node, const aiScene* scene) {
	std::cout << "SUCCESS::ASSIMP::NODE::PROCESSING" << std::endl;

	// loop on meshes and add them
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
	// loop on children nodes and process them
	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}


}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::cout << "SUCCESS::ASSIMP::MESH::PROCESSING" << std::endl;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//vertices
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 v;

		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;

		vertex.Position = v;

		v.x = mesh->mNormals[i].x;
		v.y = mesh->mNormals[i].y;
		v.z = mesh->mNormals[i].z;

		vertex.Normal = v;

		if (mesh->mTextureCoords[0]) { // assimp allows up to 8 texture coords per vertex, we are only going to use 1
			glm::vec2 v2;
			v2.x = mesh->mTextureCoords[0][i].x;
			v2.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = v2;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}
	std::cout << "SUCCESS::ASSIMP::MESH::VERTICES::PROCESSED" << std::endl;

	//indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	std::cout << "SUCCESS::ASSIMP::MESH::INDICIES::PROCESSED" << std::endl;
	//textures
	std::vector<Tex> textures;
	if (mesh->mMaterialIndex >= 0) { // we first check if this mesh has an applied material, also used to retrieve the actual material from the materials array in the scene
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Tex> diffuseTextures = loadOptimizedTextures(aiTextureType_DIFFUSE, material,"diffuse");
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
		std::vector<Tex> specularTextures = loadOptimizedTextures(aiTextureType_SPECULAR, material,"specular");
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

	}
	std::cout << "SUCCESS::ASSIMP::MESH::TEXTURES::PROCCESSED" << std::endl;

	std::cout << "SUCCESS::ASSIMP::MESH::PROCCESSED" << std::endl;


	return Mesh(vertices, indices, textures);
}

unsigned int Model::TextureFromFile(std::string fileName, std::string directory) {
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	

	std::string path = directory + '/' + fileName;
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return id;
}

std::vector<Tex> Model::loadOptimizedTextures(aiTextureType type, aiMaterial* material, std::string typeStr)
{
	std::vector<Tex> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Tex texture;
			texture.id = TextureFromFile(str.C_Str(), m_Directory);
			texture.type = typeStr;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

std::string Model::getTextureStringType(aiTextureType type)
{
	std::string newType = "diffuse";
	switch (type) {
		case aiTextureType_DIFFUSE: newType = "diffuse";
		case aiTextureType_SPECULAR: newType = "specular";
	};
	return newType;
}