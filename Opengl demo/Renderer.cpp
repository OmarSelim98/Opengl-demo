#include "Renderer.h"

Renderer::Renderer(int windowWidth, int windowHeight) {
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
}

void Renderer::Clear() const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Clear(float* color) const {
	glClearColor(color[0], color[1], color[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Renderer::Draw(Model model, Shader& shader)
{
	for (unsigned int i = 0; i < model.meshes.size(); i++)
		model.meshes[i].Draw(shader);
}

void Renderer::EnableAntialiasing()
{
	isAntiAliasingEnabled = true;
	SetupAntiAliasing();
}

void Renderer::SetupAntiAliasing()
{
	/* SETUP FINAL REDNER QUAD */
	/* ----------------------- */
	SetupQuad();

	/* MAIN MULTI SAMPLE FRAME BUFFER*/
	/* ----------------------------- */
	// setup multipsampled framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//COLOR BUFFER
	glGenTextures(1, &msColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, mWindowWidth, mWindowHeight, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msColorBuffer, 0);

	//DEPTH/STENCIL BUFFER
	unsigned int msRBO;
	glGenRenderbuffers(1, &msRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, msRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, mWindowWidth, mWindowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "ERROR::FRAMEBUFFER::INCOMPLETE" << endl;
	}

	/* INTERMEDIATE BUFFER */
	/* ------------------- */
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	//COLOR BUFFER ONLY
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWindowWidth, mWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* QUAD SHADER */
	/* ----------- */
	quadShader.Bind();
	quadShader.setInt("screenTexture", 0);
}

void Renderer::SetupQuad() {
	float  quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// setup screen VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Renderer::BindMainFrameBuffer() {
	if (isAntiAliasingEnabled) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
void Renderer::StartAntialiasing() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	Clear();
	glEnable(GL_DEPTH_TEST);
}
void Renderer::EndAntialaising() {
	isAntiAliasingEnabled = false;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
	glBlitFramebuffer(0, 0, mWindowWidth, mWindowHeight, 0, 0, mWindowWidth, mWindowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	RenderToQuad();
}
void Renderer::RenderToQuad() {
	BindMainFrameBuffer();
	Clear();
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(quadVAO);
	quadShader.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}