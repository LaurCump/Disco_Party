#include "lab_m1/Tema3/Tema3_h/dancer.h"
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

Dancer::Dancer(glm::vec3 pos, glm::vec3 size, World_object::Material material,
    int currentTopDownViewMatrixPosition, float moveSpeed) : World_object(pos, size, material)
{
    this->currentTopDownViewMatrixPosition = currentTopDownViewMatrixPosition;
    this->moveSpeed = moveSpeed;
    xOzDirection[0] = 1; xOzDirection[1] = 0;  // Indreptandu-se pe X pozitiv
    turnTimerSeconds = 0.0f;
}

Dancer::~Dancer()
{
}

void Dancer::ChangeDirection()
{
    // Alege o noua directie random
   xOzDirection[0] = rand() % 2 == 0 ? -1 : 1;
   xOzDirection[1] = rand() % 2 == 0 ? -1 : 1;

    int dir_helper = rand() % 3;
    switch (dir_helper)
    {
    case 0:  // miscarea pe axa Oz
       xOzDirection[0] = 0.0f;
        break;
    case 1:  // miscarea pe axa Ox
       xOzDirection[1] = 0.0f;
        break;
    default:  // miscarea pe diagonala (Ox si Oz)
        break;
    }
}

void Dancer::ResetTurnTimer()
{
    turnTimerSeconds = 0.0f;
}

void Dancer::UpdateTurnTimer(float deltaTimeSeconds)
{
    turnTimerSeconds += deltaTimeSeconds;
}

bool Dancer::MustTurn()
{
    return turnTimerSeconds >= turnTimerForSeconds;
}

void Dancer::Move(glm::vec3 movement)
{
    pos += movement;
}
