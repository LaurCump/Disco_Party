#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class World_object
    {
    public:
        struct Material
        {
            glm::vec4 color;
            float kd;       // reflexia difuza de material constant
            float ks;       // reflexia speculara de material constant
            int shininess;  // material shininess
            float ambient_light;
        };

    public:
        World_object(glm::vec3 pos = glm::vec3(0), glm::vec3 size = glm::vec3(1), glm::vec4 color = glm::vec4(0));
        World_object(glm::vec3 pos, glm::vec3 size, Material material);
        World_object(glm::vec3 pos, glm::vec2 discoMatPos, glm::vec3 size, Material material);
        ~World_object();

    public:
        glm::vec3 pos;
        glm::vec3 size;
        Material material;

        // Daca placa podelei de dans contine pozitia i, j in matricea de view de sus in jos a ringului de dans
        glm::vec2 discoMatPos;
    };
}
