#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID; //program ID

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vStream, fStream;

			vStream << vShaderFile.rdbuf();
			fStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vStream.str();
			fragmentCode = fStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		unsigned int vertexID = CreateShader(GL_VERTEX_SHADER, vertexCode.c_str());
		unsigned int fragmentID= CreateShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

		ID = CreateProgram(vertexID,fragmentID,true);
	}

	void Bind()
	{
		glUseProgram(ID);
	}
	void UnBind()
	{
		glUseProgram(0);
	}

	void setBool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, glm::mat4 value) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE, glm::value_ptr(value));
	}
	void setVec3(const std::string& name, glm::vec3 value) {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
	}
private:

	unsigned int CreateShader(unsigned int shaderType, const char* shaderSource) {
		int status;
		char info[512];

		unsigned int shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &shaderSource, NULL);
		glCompileShader(shaderID);
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(shaderID, 512, NULL, info);
			std::cout << "ERROR :: SHADER :: " << ((shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << " :: COMPILATION FAILED\n" << info << std::endl;
		}

		return shaderID;
	}
	unsigned int CreateProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID, bool deleteShaders) {
		int success;
		char infoLog[512];
		unsigned int programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR :: SHADER :: PROGRAM :: LINKING FAILED\n" << infoLog << std::endl;
		}
		if (deleteShaders) {
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
		}
		return programID;
	}
};

#endif