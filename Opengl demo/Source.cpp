#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader_class.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);

float BACKGROUND_COLOR[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float brightness = 0.5f;
float vertices[] = {
	//pos			 //color				//texcoords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

unsigned int VAO, VBO, EBO;

// for texture
unsigned char* imgData;
unsigned int texture1, texture2;

void changeMixRatio(Shader * shaderInstance) {
	float timeSinceStart = glfwGetTime();
	float mixValue = (sin(timeSinceStart) / 4) + 0.333;
	shaderInstance->use();
	shaderInstance->setFloat("mixValue",mixValue);
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

	Shader shader("basic_shader.vert", "basic_shader.frag");

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* VERTEX ARRAY / VERTEX BUFFER */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set & enable vertex attributes
	//1st : position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//2nd : color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//3rd: texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//Texture
	int width, height, nChannles;
	imgData = stbi_load("ori.jpg", &width, &height, &nChannles, 0);

	if (imgData == NULL) {
		std::cout << "Failed to lead texture" << std::endl;
		return -1;
	}
	glGenTextures(1, &texture1); // generate a 2d texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1); // bind the generated 2d texture to the state

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BACKGROUND_COLOR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData); //specify the 2d texture
	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap for the bound 2d texture

	stbi_image_free(imgData);

	imgData = stbi_load("ori-and-niro.png", &width, &height, &nChannles, 4);

	if (imgData == NULL) {
		std::cout << "Failed to lead texture" << std::endl;
		return -1;
	}
	glGenTextures(1, &texture2); // generate a 2d texture
	glBindTexture(GL_TEXTURE_2D, texture2); // bind the generated 2d texture to the state

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData); //specify the 2d texture
	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap for the bound 2d texture

	stbi_image_free(imgData);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	/* Render Config */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */

	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		// Rendering commands start
		glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], BACKGROUND_COLOR[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		changeMixRatio(&shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		shader.use();
		shader.setFloat("brightness", brightness);

		// draw first triangle
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Rendering commands end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean resources on loop's end.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if(brightness < 1.0f)
		{
			brightness += 0.1f;
			std::cout << brightness << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (brightness > 0.1f)
		{
			brightness -= 0.1f;
			std::cout << brightness << std::endl;
		}
	}
}