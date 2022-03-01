#pragma once

#include "components/simple_scene.h"
#include "world_object.h"

namespace m1
{
    class Dancer : public m1::World_object
    {
    public:
        Dancer(glm::vec3 pos, glm::vec3 size, World_object::Material material,
            int currentTopDownViewMatrixPosition, float moveSpeed = 1.0f);
        ~Dancer();
        void ChangeDirection();
        void ResetTurnTimer();
        void UpdateTurnTimer(float deltaTimeSeconds);
        bool MustTurn();
        void Move(glm::vec3 movement);

    public:
        float moveSpeed;
        float xOzDirection[2];  // [0] este .x si [1] este .z

        float turnTimerSeconds;  // Timpul pentru schimbarea directiei
        const float turnTimerForSeconds = 2.0f;

        int currentTopDownViewMatrixPosition;
    };
}
