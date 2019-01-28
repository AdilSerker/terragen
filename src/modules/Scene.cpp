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
	glm::mat4 light_view = glm::lookAt(camera->target + light->position, camera->target, glm::vec3(0, 1, 0));
	glm::mat4 light_proj = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 10.0f, 10000.0f);

	glm::vec3 light_direction = glm::normalize(light->target - light->position);

	glUseProgram(shader->program);

	glUniformMatrix4fv(glGetUniformLocation(shader->program, "view"), 1, GL_FALSE, glm::value_ptr(camera->view_matrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "proj"), 1, GL_FALSE, glm::value_ptr(camera->proj_matrix()));
	glUniform3f(glGetUniformLocation(shader->program, "light_dir"), light_direction.x, light_direction.y, light_direction.z);

	glUniformMatrix4fv(glGetUniformLocation(shader->program, "light_view"), 1, GL_FALSE, glm::value_ptr(light_view));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "light_proj"), 1, GL_FALSE, glm::value_ptr(light_proj));

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, light->tex);
	glUniform1i(glGetUniformLocation(shader->program, "shadows"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, heightmap->vbo);

	glEnableVertexAttribArray(glGetAttribLocation(shader->program, "vPosition"));
	glEnableVertexAttribArray(glGetAttribLocation(shader->program, "vNormal"));
	glEnableVertexAttribArray(glGetAttribLocation(shader->program, "vAO"));

	glVertexAttribPointer(glGetAttribLocation(shader->program, "vPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void *)(sizeof(float) * 0));
	glVertexAttribPointer(glGetAttribLocation(shader->program, "vNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void *)(sizeof(float) * 3));
	glVertexAttribPointer(glGetAttribLocation(shader->program, "vAO"), 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void *)(sizeof(float) * 6));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heightmap->tbo);

	glDrawElements(GL_TRIANGLES, ((heightmap->data.size() - 1) / 2) * ((heightmap->data[0].size() - 1) / 2) * 2 * 3, GL_UNSIGNED_INT, (void *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(glGetAttribLocation(shader->program, "vPosition"));
	glDisableVertexAttribArray(glGetAttribLocation(shader->program, "vNormal"));
	glDisableVertexAttribArray(glGetAttribLocation(shader->program, "vAO"));

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