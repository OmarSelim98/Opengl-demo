#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);

const char* vertexShaderSource =  "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor= vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

float vertices[] = {
	-0.5f, -0.5f, 0.0f,	// bottom left
	0.5f, -0.5f, 0.0f,	// bottom right
	0.5f, 0.5f, 0.0f,	//top right
	-0.5f, 0.5f, 0.0f	//top left
};

unsigned int indices[] = {
	0 , 1 , 2,
	0 , 2 , 3
};

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;


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

	//fragment shader
	fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	//shader program
	shaderProgram = CreateProgram(&vertexShader, &fragmentShader, true);
	/* ========== */

	/* VERTEX ARRAY / VERTEX BUFFER */
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//elements buffer object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set & enable vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0); // It is safe here to unbind VBO before unbinding VAO, as glVertexAttribPointer already registers the vertex attribute's bound buffer (VBO in this case).
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	/* ========== */

	/* Render Config */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */

	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		// Rendering commands start
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // use EBO to draw triangles.
		glBindVertexArray(0);

		// Rendering commands end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean resources on loop's end.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
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