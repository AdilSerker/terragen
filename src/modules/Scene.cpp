#include "Scene.h"
#include <iostream>

Scene::Scene()
{
	this->shader = new Shader();
	shader->load("./shaders/terrain.vs", "./shaders/terrain.fs");

	this->heightmap = new Heightmap();
}

Scene::~Scene()
{
	delete shader;
	delete heightmap;
}

void Scene::draw(LightDirectional *light, CameraOrbit *camera)
{
	glUseProgram(shader->program);

	glm::mat4 view = camera->view_matrix();
	glm::vec4 lp = view * glm::vec4(light->position, 1.0);

	glUniform3f(glGetUniformLocation(shader->program, "Ld"), 0.9f, 0.5f, 0.3f);
	glUniform3f(glGetUniformLocation(shader->program, "Kd"), 1.0f, 1.0f, 1.0f);
	glUniform4f(glGetUniformLocation(shader->program, "LightPosition"), lp.x, lp.y, lp.z, lp.w);

	glm::mat4 mv = view * glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mv));

	glm::mat3 nm = glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));
	glUniformMatrix3fv(glGetUniformLocation(shader->program, "NormalMatrix"), 1, GL_FALSE,
					   glm::value_ptr(nm));

	glUniformMatrix4fv(glGetUniformLocation(shader->program, "MVP"), 1, GL_FALSE,
					   glm::value_ptr(camera->proj_matrix() * mv));

	glBindBuffer(GL_ARRAY_BUFFER, heightmap->vbo);

	glEnableVertexAttribArray(glGetAttribLocation(shader->program, "vPosition"));
	glEnableVertexAttribArray(glGetAttribLocation(shader->program, "vNormal"));

	glVertexAttribPointer(glGetAttribLocation(shader->program, "vPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void *)(sizeof(float) * 0));
	glVertexAttribPointer(glGetAttribLocation(shader->program, "vNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void *)(sizeof(float) * 3));

	glDrawElements(GL_TRIANGLES, ((heightmap->data.size() - 1) / 2) * ((heightmap->data[0].size() - 1) / 2) * 2 * 3, GL_UNSIGNED_INT, (void *)0);

	glDisableVertexAttribArray(glGetAttribLocation(shader->program, "vPosition"));
	glDisableVertexAttribArray(glGetAttribLocation(shader->program, "vNormal"));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

void Scene::generate()
{
	heightmap->generate(pow(2, 10) + 1, 1.0, 1.0);
}

void Scene::save()
{
	heightmap->save();
}

void Scene::smooth()
{
	heightmap->smooth(5);
}