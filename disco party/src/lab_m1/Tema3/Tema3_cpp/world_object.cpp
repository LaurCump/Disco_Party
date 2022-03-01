#include "lab_m1/Tema3/Tema3_h/world_object.h"
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

World_object::World_object(glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	this->pos = pos;
	this->size = size;
	this->material.color = color;
	this->material.kd = 0.5f;
	this->material.ks = 0.1f;
	this->material.shininess = 30;
	this->material.ambient_light = 0.25f;
	this->discoMatPos = glm::vec2(0);
}

World_object::World_object(glm::vec3 pos, glm::vec3 size, Material material)
{
	this->pos = pos;
	this->size = size;
	this->material.color = material.color;
	this->material.kd = material.kd;
	this->material.ks = material.ks;
	this->material.shininess = material.shininess;
	this->material.ambient_light = material.ambient_light;
	this->discoMatPos = glm::vec2(0);
}

World_object::World_object(glm::vec3 pos, glm::vec2 discoMatPos, glm::vec3 size, Material material)
{
	this->pos = pos;
	this->size = size;
	this->material.color = material.color;
	this->material.kd = material.kd;
	this->material.ks = material.ks;
	this->material.shininess = material.shininess;
	this->material.ambient_light = material.ambient_light;
	this->discoMatPos = discoMatPos;
}


World_object::~World_object()
{
}
