#include "SkyboxRenderer.h"

const float SkyboxRenderer::SIZE =500.f;
std::vector<float> SkyboxRenderer::VERTICES = {
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE
};

const std::vector<string> SkyboxRenderer::TEXTURE_FILES = { "right.png","left.png","top.png","bottom.png","back.png","front.png" };
const std::vector<string> SkyboxRenderer::NIGHT_TEXTURE_FILES = { "nightRight.png","nightLeft.png","nightTop.png","nightBottom.png","nightBack.png","nightFront.png" };

SkyboxRenderer::SkyboxRenderer(Loader loader, Camera camera){
	cube = loader.loadToVAO(VERTICES,3);
	texture = loader.loadCubeMap(TEXTURE_FILES);
	nightTexture = loader.loadCubeMap(NIGHT_TEXTURE_FILES);
	shader = new SkyboxShader();
	shader->start();
	shader->connectTextureUnits();
	shader->loadProjectionMatrix(camera);
	shader->stop();
}

void SkyboxRenderer::render(Camera camera, float r, float g, float b){
  
	shader->start();
	
	shader->loadViewMatrix(camera);
	shader->loadFogColor(r, g, b);
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(cube->vaoID);
	glEnableVertexAttribArray(0);
	bindTextures();
	glDrawArrays(GL_TRIANGLES, 0, cube->vertexCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	shader->stop();


}

void SkyboxRenderer::bindTextures() {
	time += WindowManager::getFrameTimeSeconds() * 70;
	time = fmod(time,24000.);
	GLuint texture1;
	GLuint texture2;
	float blendFactor;
	if (time >= 0 && time < 5000) {
		texture1 = nightTexture;
		texture2 = nightTexture;
		blendFactor =1.f;
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = texture;
		texture2 = nightTexture;
		blendFactor = 1-(time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = texture;
		texture2 = texture;
		blendFactor = 0.f;
	}
	else {
		texture1 = texture;
		texture2 = nightTexture;
		blendFactor = (time - 21000) / (24000 - 21000);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
	shader->loadBlendFactor(blendFactor);
}

void SkyboxRenderer::cleanUp()
{
	shader->cleanUp();
}
