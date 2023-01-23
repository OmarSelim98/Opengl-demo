#pragma once
#include "Model.h"

class Renderer {
public:
	Renderer(int windowwidth, int windowHeight);
	void Clear() const;
	void Clear(float* color) const;
	void Draw(Model model, Shader& shader);
	void DrawInstanced(Model model, unsigned int instancesCount);
	void EnableAntialiasing();
	void StartAntialiasing();
	void EndAntialaising();
	void BindMainFrameBuffer();
private:
	bool isAntiAliasingEnabled = false;
	int mWindowWidth;
	int mWindowHeight;
	unsigned int frameBuffer;
	unsigned int msColorBuffer;
	unsigned int intermediateFBO;
	unsigned int screenTexture;
	unsigned int quadVAO, quadVBO;

	
	Shader quadShader = Shader("quad_shader.vert", "quad_shader.frag");
	
	void SetupAntiAliasing();
	void SetupQuad();
	void RenderToQuad();
};