#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_class.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);


float first_triangle[] = {
	//positions			//colors
	-0.5f, 0.0f, 0.0f,	1.0f, 0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,	0.0f, 1.0f, 0.5f,
	0.0f, 0.0f, 0.0f,	0.5f, 0.0f, 1.0f
};

unsigned int VAOs[2];
unsigned int VBOs[2];
unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader[2];
unsigned int shaderProgram[2];



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

	Shader shader("basic_shader.vs", "basic_shader.fs");

	/* VERTEX ARRAY / VERTEX BUFFER */
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//FIRST TRIANGLE
	ConfigureTriangle(&first_triangle, 0);

	/* Render Config */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */

	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		// Rendering commands start
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		// draw first triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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