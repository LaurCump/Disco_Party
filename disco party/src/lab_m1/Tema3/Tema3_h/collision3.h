#pragma once

namespace Collision3
{
    bool AABBAABB(glm::vec3 aMinPos, glm::vec3 aMaxPos, glm::vec3 bMinPos, glm::vec3 bMaxPos)
    {
        return (aMinPos.x <= bMaxPos.x && aMaxPos.x >= bMinPos.x) &&
            (aMinPos.y <= bMaxPos.y && aMaxPos.y >= bMinPos.y) &&
            (aMinPos.z <= bMaxPos.z && aMaxPos.z >= bMinPos.z);
    }
}
