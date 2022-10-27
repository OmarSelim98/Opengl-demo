#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource[2] = { "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"FragColor= vec4(ourColor, 1.0f);\n"
"}\0",  
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 uniformColor;\n"
"void main()\n"
"{\n"
"FragColor= uniformColor;\n"
"}\0" };

float first_triangle[] = {
	//positions			//colors
	-0.5f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f
};
//float second_triangle[] = {
//	0.0f, 0.0f, 0.0f,
//	0.5f, 0.5f, 0.0f,
//	0.5f, 0.0f, 0.0f };
////
//unsigned int indices[] = {
//	0 , 1 , 2,
//	0 , 2 , 3
//};

unsigned int VAOs[2];
unsigned int VBOs[2];
unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader[2];
unsigned int shaderProgram[2];


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
unsigned int CreateProgram(unsigned int* vertexShaderID, unsigned int* fragmentShaderID, bool deleteShaders) {
	int success;
	char infoLog[512];
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, *vertexShaderID);
	glAttachShader(programID, *fragmentShaderID);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR :: SHADER :: PROGRAM :: LINKING FAILED\n" << infoLog << std::endl;
	}
	if (deleteShaders) {
		glDeleteShader(*vertexShaderID);
		glDeleteShader(*fragmentShaderID);
	}
	return programID;
}

void ChangeColor(unsigned int programID) {
	/*MAKE SURE TO glUseProgram BEFORE TRYING TO UPDATE THE UNIFORM LOCATION*/

	//change colors
	float timeVal = (float) glfwGetTime(); // Seconds since the program has started.

	float redVal = (sin(timeVal) / 2) + 0.5f;
	float greenVal = (sin(timeVal) / 4) + 0.5f;
	float blueVal = (sin(timeVal+0.25) / 2) + 0.5f;

	//get uniform.
	unsigned int uniformLocation = glGetUniformLocation(programID, "uniformColor");
	
	// Update Uniform
	if (uniformLocation != -1) {
		glUniform4f(uniformLocation, redVal, greenVal, blueVal, 1.0f);
	}
}

void ConfigureTriangle(float(*triangle)[18], unsigned int buffer_index) {
	glBindVertexArray(VAOs[buffer_index]);

	//vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[buffer_index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*triangle), *triangle, GL_STATIC_DRAW);

	//set & enable vertex attributes
	//1st : position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//2nd : color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3*sizeof(float)) );
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // It is safe here to unbind VBO before unbinding VAO, as glVertexAttribPointer already registers the vertex attribute's bound buffer (VBO in this case).
	glBindVertexArray(0);
}

int main() {

	//Initiate GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window and set it as a context to the current thread.
	//Abort if the window can't be created.
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGl Demo", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to initialize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	//Adjust viewport to the resized window.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Abort if glad hasn't loaded all opengl's functions pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* SHADERS PREPARATION */
	//vertex shader
	vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);

	//fragment shaders
	fragmentShader[0] = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource[0]);
	//fragmentShader[1] = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource[1]);

	//shader programs
	shaderProgram[0] = CreateProgram(&vertexShader, &(fragmentShader[0]), true);
	//shaderProgram[1] = CreateProgram(&vertexShader, &(fragmentShader[1]), true);
	/* ========== */

	/* VERTEX ARRAY / VERTEX BUFFER */
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//FIRST TRIANGLE
	ConfigureTriangle(&first_triangle, 0);


	//Second TRIANGLE
	//ConfigureTriangle(&second_triangle, 1);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	/* ========== */

	/* Render Config */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */

	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		// Rendering commands start
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram[0]);

		// draw first triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glUseProgram(shaderProgram[1]);

		//ChangeColor(shaderProgram[1]);
		//// draw second triangle
		//glBindVertexArray(VAOs[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // use EBO to draw triangles.

		glBindVertexArray(0);

		// Rendering commands end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean resources on loop's end.
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}