#pragma once

#include "components/simple_scene.h"

#include "world_object.h"

namespace m1
{
    class World_light : public World_object
    {
    public:
        World_light(glm::vec3 pos = glm::vec3(0), glm::vec3 size = glm::vec3(1), glm::vec4 color = glm::vec4(0), bool isSpotlight = false);
        World_light(glm::vec3 pos, glm::vec3 size, Material material, bool isSpotlight = false);
        World_light(glm::vec3 pos, glm::vec2 discoMatPos, glm::vec3 size, World_object::Material material, bool isSpotlight = false);
        ~World_light();

        void ChangeDirection();
        void ResetTurnTimer();
        void UpdateTurnTimer(float deltaTimeSeconds);
        bool MustTurn();
        void Move(float deltaTimeSeconds);

    public:
        // Doar pentru tipul de reflector
        glm::vec3 lightDirection;

        const glm::vec3 initialLightDirection{ glm::vec3(0, -1, 0) };

        float oxAngleRadians;
        float ozAngleRadians;
        const float maxAngleDegrees = 10.0f;

        float xOzDirection[2];  // [0] este .x si [1] este .z

        float turnTimerSeconds;  // Timpul pentru schimbarea directiei
        const float turnTimerForSeconds = 2.0f;

        glm::vec3 lightColor;
        bool isSpotlight;
    };
}
