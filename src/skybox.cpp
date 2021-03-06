#include "skybox.h"

static vector<float> vertex = {
		-1000.0f, 1000.0f, -1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		1000.0f, 1000.0f, -1000.0f,
		-1000.0f, 1000.0f, -1000.0f,

		-1000.0f, -1000.0f, 1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, 1000.0f, -1000.0f,
		-1000.0f, 1000.0f, -1000.0f,
		-1000.0f, 1000.0f, 1000.0f,
		-1000.0f, -1000.0f, 1000.0f,

		1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,

		-1000.0f, -1000.0f, 1000.0f,
		-1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, -1000.0f, 1000.0f,
		-1000.0f, -1000.0f, 1000.0f,

		-1000.0f, 1000.0f, -1000.0f,
		1000.0f, 1000.0f, -1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		-1000.0f, 1000.0f, 1000.0f,
		-1000.0f, 1000.0f, -1000.0f,

		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f, 1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f, 1000.0f,
		1000.0f, -1000.0f, 1000.0f
};

static vector<const char*> cubeMapDayFileName = {
	"./skybox/dayRight.png",
	"./skybox/dayLeft.png",
	"./skybox/dayTop.png",
	"./skybox/dayBottom.png",
	"./skybox/dayBack.png",
	"./skybox/dayFront.png"
};

static vector<const char*> cubeMapNightFileName = {
	"./skybox/nightRight.png",
	"./skybox/nightLeft.png",
	"./skybox/nightTop.png",
	"./skybox/nightBottom.png",
	"./skybox/nightBack.png",
	"./skybox/nightFront.png"
};

void SkyboxRenderer::initGL()
{
	glGenVertexArrays(1, &_cube);
	glBindVertexArray(_cube);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float),
						vertex.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	_cubeMapDay = LoadCubeMap(cubeMapDayFileName);
	// _cubeMapNight = LoadCubeMap(cubeMapNightFileName);
}

void SkyboxRenderer::render() const
{
    glDepthFunc(GL_LEQUAL);

	glBindVertexArray(_cube);
	glEnableVertexAttribArray(0);

	// day cubemap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapDay);
	_shader->setCubeMapDay(1);

	// night cubemap
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapNight);
	_shader->setCubeMapNight(0);

	// blend day and night
	static float blend = 0;
	float blendFactor = cos(blend);
	if(blendFactor < 0) blendFactor = -blendFactor;
	_shader->setBlendFactor(blendFactor);
	blend += 0.0002f;

	// rotate
	static float rotate = 0.0f;
	static glm::vec3 __yaxis = glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), rotate, __yaxis);
	_shader->setModelMatrix(modelMatrix);
	rotate += 0.0005f;

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}