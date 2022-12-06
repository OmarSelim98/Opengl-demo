#pragma once

#include <glad/glad.h>

struct HELPER {
	static unsigned int sizeOfGL(unsigned int t) {
		switch (t)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_UNSIGNED_INT:	return  sizeof(unsigned int);
		case GL_UNSIGNED_BYTE:	return sizeof(char);
		default:				return 0;
		}
	}
};