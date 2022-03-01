#include "lab_m1/Tema3/Tema3_h/world_light.h"
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

World_light::World_light(glm::vec3 pos, glm::vec3 size, glm::vec4 color, bool isSpotlight) : World_object(pos, size, color)
{
	this->isSpotlight = isSpotlight;
	lightDirection = initialLightDirection;
	oxAngleRadians = 0.0f;
	ozAngleRadians = 0.0f;

	if (material.color.r == 0 && material.color.g == 0 && material.color.b == 0)
	{
		lightColor = glm::vec3(1);	// culoarea alba
	}
	else
	{
		lightColor.r = material.color.r;
		lightColor.g = material.color.g;
		lightColor.b = material.color.b;
	}

	xOzDirection[0] = 1; xOzDirection[1] = 0;  // Indreptand-se spre X pozitiv
	turnTimerSeconds = 0.0f;
}

World_light::World_light(glm::vec3 pos, glm::vec3 size, Material material, bool isSpotlight) : World_object(pos, size, material)
{
	this->isSpotlight = isSpotlight;
	lightDirection = initialLightDirection;
	oxAngleRadians = 0.0f;
	ozAngleRadians = 0.0f;

	if (material.color.r == 0 && material.color.g == 0 && material.color.b == 0)
	{
		lightColor = glm::vec3(1);	 // culoarea alba
	}
	else
	{
		lightColor.r = material.color.r;
		lightColor.g = material.color.g;
		lightColor.b = material.color.b;
	}

	xOzDirection[0] = 1; xOzDirection[1] = 0;  // Indreptand-se spre X pozitiv
	turnTimerSeconds = 0.0f;
}

World_light::World_light(glm::vec3 pos, glm::vec2 discoMatPos, glm::vec3 size, World_object::Material material, bool isSpotlight) : World_object(pos, discoMatPos, size, material)
{
	this->isSpotlight = isSpotlight;
	lightDirection = glm::vec3(0, -1, 0);
	oxAngleRadians = 0.0f;
	ozAngleRadians = 0.0f;

	if (material.color.r == 0 && material.color.g == 0 && material.color.b == 0)
	{
		lightColor = glm::vec3(1);	// culoarea alba
	}
	else
	{
		lightColor.r = material.color.r;
		lightColor.g = material.color.g;
		lightColor.b = material.color.b;
	}

	xOzDirection[0] = 1; xOzDirection[1] = 0;  // Indreptand-se spre X pozitiv
	turnTimerSeconds = 0.0f;
}


World_light::~World_light()
{
}

void World_light::ChangeDirection()
{
	// Alege random o noua directie.
	xOzDirection[0] = rand() % 2 == 0 ? -1 : 1;
	xOzDirection[1] = rand() % 2 == 0 ? -1 : 1;

	int dir_helper = rand() % 3;
	switch (dir_helper)
	{
	case 0:  // miscarea pe axa Oz
		xOzDirection[0] = 0.0f;
		break;
	case 1:  //  miscarea pe axa Ox
		xOzDirection[1] = 0.0f;
		break;
	default:  // miscarea pe diagonala (Ox si Oz)
		break;
	}
}

void World_light::ResetTurnTimer()
{
	turnTimerSeconds = 0.0f;
}

void World_light::UpdateTurnTimer(float deltaTimeSeconds)
{
	turnTimerSeconds += deltaTimeSeconds;
}

bool World_light::MustTurn()
{
	return turnTimerSeconds >= turnTimerForSeconds;
}

void World_light::Move(float deltaTimeSeconds)
{
	float speed = 0.1f;
	float radiansAngle = deltaTimeSeconds * speed;
	glm::vec3 newLightDirection{ lightDirection };
	float newOxAngleRadians;
	float newOzAngleRadians;
	glm::vec3 refDir{};


	if (xOzDirection[0] == -1)
	{ 
		// Miscarea pe OX (Stanga)
		newLightDirection = glm::vec3(glm::rotate(glm::mat4(1), -radiansAngle, glm::vec3(0, 0, 1)) * glm::vec4(newLightDirection, 1));
	}
	else if (xOzDirection[0] == 1)
	{ 
		// Miscarea pe OX (Dreapta)
		newLightDirection = glm::vec3(glm::rotate(glm::mat4(1), radiansAngle, glm::vec3(0, 0, 1)) * glm::vec4(newLightDirection, 1));
	}

	if (xOzDirection[1] == 1)
	{  
		// Miscarea pe OZ (Sus)
		newLightDirection = glm::vec3(glm::rotate(glm::mat4(1), radiansAngle, glm::vec3(1, 0, 0)) * glm::vec4(newLightDirection, 1));
	}
	else if (xOzDirection[1] == -1)
	{  
		// Miscarea pe OZ (Jos)
		newLightDirection = glm::vec3(glm::rotate(glm::mat4(1), -radiansAngle, glm::vec3(1, 0, 0)) * glm::vec4(newLightDirection, 1));
	}


	// Calculam cat de mult i trebuie rotit pe axe individual.
	{
		refDir = glm::normalize(glm::vec3(newLightDirection.x, newLightDirection.y, 0.0f));
		newOzAngleRadians = glm::acos(glm::dot(refDir, initialLightDirection));

		// Verificam daca noul unghi in jurul lui OZ este permis inainte sa-i dam gama completa de miscare.
		if (glm::degrees(newOzAngleRadians) > maxAngleDegrees)
		{  
			// Nu este permis.
			// Schimbam directia.
			xOzDirection[0] *= -1;
			return;
		}

		if (refDir.x < 0)  // pentru gama completa de miscare
		{
			newOzAngleRadians *= -1;
		}

		refDir = glm::normalize(glm::vec3(0.0f, newLightDirection.y, newLightDirection.z));
		newOxAngleRadians = glm::acos(glm::dot(refDir, initialLightDirection));

		// Verificam daca noul unghi in jurul lui OX este permis inainte sa-i dam gama completa de miscare.
		if (glm::degrees(newOxAngleRadians) > maxAngleDegrees)
		{  
			// Nu este permis.
			// Schimbam directia.
			xOzDirection[1] *= -1;
			return;
		}

		if (refDir.z > 0)  // pentru gama completa de miscare
		{
			newOxAngleRadians *= -1;
		}
	}

	// Aplicam schimbarile.
	oxAngleRadians = newOxAngleRadians;
	ozAngleRadians = newOzAngleRadians;
	lightDirection = newLightDirection;
}
