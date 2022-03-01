#include "lab_m1/Tema3/Tema3_h/level3.h"
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema3/Tema3_h/collision3.h"

using namespace m1;

Level3::Level3()
{
    danceFloorSize = 8;     // gridul are danceFloorSize * danceFloorSize dimensiunea

    CreateDanceFloor();
    CreateDiscoBall();
    CreateSpotlights();
    CreateDancers();
}

Level3::~Level3()
{
    for each (World_object *w in danceFloor)
    {
        delete w;
    }
    danceFloor.clear();

    for each (World_object *d in dancers)
    {
        delete d;
    }
    dancers.clear();
}

// Crearea globului disco
void Level3::CreateDiscoBall()
{
    World_object::Material material;
    material.color = glm::vec4(0);   // nicio culoare, niciun alfa (este transparent)
    material.kd = 0.5f;
    material.ks = 0.5f;
    material.shininess = 30;
    material.ambient_light = 1;  // nu conteaza

    glm::vec3 size{ glm::vec3(1) };
    int mid_box_idx = (danceFloorSize * danceFloorSize) / 2 + danceFloorSize / 2;
    glm::vec3 pos{ danceFloor.at(mid_box_idx)->pos - danceFloor.at(mid_box_idx)->size / 2.0f };
    pos.y = danceFloorWalls.at(0)->size.y - size.y / 2.0f;

    discoBall = new World_light(pos, size, material, false);
}

// Crearea ringului de dans
void Level3::CreateDanceFloor()
{
    glm::vec3 floorBoxSize{ glm::vec3(1, 0.2f, 1) };
    glm::vec3 firstBoxPos{ glm::vec3(floorBoxSize.x / 2.0f, -floorBoxSize.y / 2.0f, floorBoxSize.z / 2.0f) };
    glm::vec3 pos{ firstBoxPos };
    glm::vec4 color;

    for (int i = 0; i < danceFloorSize; ++i)
    {
        for (int j = 0; j < danceFloorSize; ++j)
        {
            World_object::Material material;
            material.color = glm::vec4( (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
            material.kd = 150.5f;
            material.ks = 0.5f;
            material.shininess = 30;
            material.ambient_light = 0.85f;

            pos.x = firstBoxPos.x + j * floorBoxSize.x;
            pos.z = firstBoxPos.x + i * floorBoxSize.z;
            World_light* d = new World_light(pos, glm::vec2(i, j), floorBoxSize, material, false);
            danceFloor.push_back(d);
        }
    }

    CreateFloorWalls();
}

// Crearea zidurilor
void Level3::CreateFloorWalls()
{
    glm::vec3 firstBoxPos{ danceFloor.at(0)->pos };
    glm::vec3 firstBoxSize{ danceFloor.at(0)->size };

    World_object::Material material;
    material.color = glm::vec4(0.001f, 0.001f, 0.001f, 1.0f);
    material.kd = 350000;
    material.ks = 0.5f;
    material.shininess = 15;
    material.ambient_light = 0.0f;

    // Peretele de sus cu vederea de sus in jos
    glm::vec3 wallBoxSize{ glm::vec3(firstBoxSize.x * danceFloorSize, 4, 1) };
    glm::vec3 pos{ firstBoxPos };
    pos.x += wallBoxSize.x / 2.0f - firstBoxSize.x / 2.0f;
    pos.y += wallBoxSize.y / 2.0f + firstBoxSize.y / 2.0f;
    pos.z -= firstBoxSize.z;

    World_object* d = new World_object(pos, wallBoxSize, material);
    danceFloorWalls.push_back(d);

    // Peretele de jos cu vederea de sus in jos
    pos.z += firstBoxSize.z * (danceFloorSize + 1);

    d = new World_object(pos, wallBoxSize, material);
    danceFloorWalls.push_back(d);

    // Peretele din stanga cu vederea de sus in jos
    wallBoxSize = glm::vec3(1, 4, firstBoxSize.x * danceFloorSize);
    pos = firstBoxPos;
    pos.x -= firstBoxSize.z;
    pos.y += wallBoxSize.y / 2.0f + firstBoxSize.y / 2.0f;
    pos.z += firstBoxSize.z * (danceFloorSize / 2) - firstBoxSize.z / 2.0f;

    d = new World_object(pos, wallBoxSize, material);
    danceFloorWalls.push_back(d);

    // Peretele din dreapta cu vederea de sus in jos
    pos.x += firstBoxSize.x * (danceFloorSize + 1);
    d = new World_object(pos, wallBoxSize, material);
    danceFloorWalls.push_back(d);

    CreateDiscoCeiling();
}

// Crearea tavanului 
void Level3::CreateDiscoCeiling()
{
    glm::vec3 firstBoxPos{ danceFloor.at(0)->pos };
    glm::vec3 firstBoxSize{ danceFloor.at(0)->size };

    float height = 4;

    glm::vec3 ceilingBoxSize{ glm::vec3(danceFloorSize, 0.1f, danceFloorSize) };
    glm::vec3 pos{ firstBoxPos };
    pos.x += ceilingBoxSize.x / 2.0f - firstBoxSize.x / 2.0f;
    pos.y += height + firstBoxSize.y / 2.0f + ceilingBoxSize.y / 2.0f;
    pos.z += ceilingBoxSize.z / 2.0f - firstBoxSize.z / 2.0f;

    World_object::Material material;
    material.color = glm::vec4(0.001f, 0.001f, 0.001f, 1.0f);
    material.kd = 350000;
    material.ks = 0.5f;
    material.shininess = 30;
    material.ambient_light = 0.0f;

    discoCeiling = new World_object(pos, ceilingBoxSize, material);
}

// Crearea dansatorilor
void Level3::CreateDancers()
{
    {
        World_object::Material material;
        material.color = glm::vec4( (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, 1.0f);
        material.kd = 350.5f;
        material.ks = 0.1f;
        material.shininess = 5;
        material.ambient_light = 0.0f;

        glm::vec3 size{ glm::vec3(0.5f, 1.0f, 0.5f) };
        int mid_box_idx = (danceFloorSize * danceFloorSize) / 2 + danceFloorSize / 2;
        glm::vec3 pos{ danceFloor.at(mid_box_idx)->pos - danceFloor.at(mid_box_idx)->size / 2.0f };
        pos.y = size.y / 2.0f;
        dancers.push_back(new Dancer(pos, size, material, mid_box_idx));
    }
    {
        World_object::Material material;
        material.color = glm::vec4( (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, 1.0f);
        material.kd = 350.5f;
        material.ks = 0.1f;
        material.shininess = 5;
        material.ambient_light = 0.0f;

        int idx = (danceFloorSize * danceFloorSize) * 0.25f + danceFloorSize * 0.25f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(0.5f, 1.0f, 0.5f) };

        pos.y = size.y / 2.0f;

        dancers.push_back(new Dancer(pos, size, material, idx));
    }
    {
        World_object::Material material;
        material.color = glm::vec4( (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, 1.0f);
        material.kd = 350.5f;
        material.ks = 0.1f;
        material.shininess = 5;
        material.ambient_light = 0.0f;

        int idx = (danceFloorSize * danceFloorSize) * 0.75f + danceFloorSize * 0.25f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(0.5f, 1.0f, 0.5f) };

        pos.y = size.y / 2.0f;

        dancers.push_back(new Dancer(pos, size, material, idx));
    }
    {
        World_object::Material material;
        material.color = glm::vec4( (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, 1.0f);
        material.kd = 350.5f;
        material.ks = 0.1f;
        material.shininess = 5;
        material.ambient_light = 0.0f;

        int idx = (danceFloorSize * danceFloorSize) * 0.25f + danceFloorSize * 0.75f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(0.5f, 1.0f, 0.5f) };

        pos.y = size.y / 2.0f;

        dancers.push_back(new Dancer(pos, size, material, idx));
    }
    {
        World_object::Material material;
        material.color = glm::vec4( (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, (26 + rand() % 230) / 255.0f, 1.0f);
        material.kd = 350.5f;
        material.ks = 0.1f;
        material.shininess = 5;
        material.ambient_light = 0.0f;

        int idx = (danceFloorSize * danceFloorSize) * 0.75f + danceFloorSize * 0.75f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(0.5f, 1.0f, 0.5f) };

        pos.y = size.y / 2.0f;

        dancers.push_back(new Dancer(pos, size, material, idx));
    }
}

// Crearea surselor de lumina
void Level3::CreateSpotlights()
{
    {
        int idx = (danceFloorSize * danceFloorSize) * 0.25f + danceFloorSize * 0.25f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(2, 5, 2) };
        glm::vec4 color{ glm::vec4( rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 0.4f) };
        
        pos.y = danceFloor.at(idx)->pos.y + size.y + danceFloor.at(idx)->size.y / 2.0f;

        spotlights.push_back(new World_light(pos, size, color, true));
    }
    {
        int idx = (danceFloorSize * danceFloorSize) * 0.75f + danceFloorSize * 0.25f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(2, 5, 2) };
        glm::vec4 color{ glm::vec4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 0.4f) };
        
        pos.y = danceFloor.at(idx)->pos.y + size.y + danceFloor.at(idx)->size.y / 2.0f;

        spotlights.push_back(new World_light(pos, size, color, true));
    }
    {
        int idx = (danceFloorSize * danceFloorSize) * 0.25f + danceFloorSize * 0.75f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(2, 5, 2) };
        glm::vec4 color{ glm::vec4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 0.4f) };

        pos.y = danceFloor.at(idx)->pos.y + size.y + danceFloor.at(idx)->size.y / 2.0f;

        spotlights.push_back(new World_light(pos, size, color, true));
    }
    {
        int idx = (danceFloorSize * danceFloorSize) * 0.75f + danceFloorSize * 0.75f;

        glm::vec3 pos{ danceFloor.at(idx)->pos - danceFloor.at(idx)->size / 2.0f };
        glm::vec3 size{ glm::vec3(2, 5, 2) };
        glm::vec4 color{ glm::vec4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 0.4f) };

        pos.y = danceFloor.at(idx)->pos.y + size.y + danceFloor.at(idx)->size.y / 2.0f;

        spotlights.push_back(new World_light(pos, size, color, true));
    }
}

// Actualizare pozitia si miscarea dansatorilor din ringul de dans
void Level3::UpdateDancers(float deltaTimeSeconds)
{
    // Actualizam cronometrul dansatorilor
    for each (Dancer *d in dancers)
    {
        if (d->MustTurn())
        {
            d->ChangeDirection();
            d->ResetTurnTimer();
        }
        d->UpdateTurnTimer(deltaTimeSeconds);
    }

    for each (Dancer *d in dancers)
    {
        // Calculam miscarea ce urmeaza sa fie executata de dansatorul d
        glm::vec3 movement
        {
            glm::vec3
            (
                d->xOzDirection[0] * d->moveSpeed * deltaTimeSeconds,
                0,
                d->xOzDirection[1] * d->moveSpeed * deltaTimeSeconds
            )
        };

        // Efectueaza coliziunea dansatorului cu peretele
        glm::vec3 box1MinPos{ d->pos + movement - glm::vec3(d->size.x) / 2.0f };
        glm::vec3 box1MaxPos{ d->pos + movement + glm::vec3(d->size.x) / 2.0f };
        for each (World_object *w in danceFloorWalls)
        {
            glm::vec3 box2MinPos{ w->pos - w->size / 2.0f };
            glm::vec3 box2MaxPos{ w->pos + w->size / 2.0f };
            if (Collision3::AABBAABB(box1MinPos, box1MaxPos, box2MinPos, box2MaxPos)) 
            {
                // Actioneaza ca o minge ce loveste un perete
                d->xOzDirection[0] *= -1;
                d->xOzDirection[1] *= -1;
                movement *= -1;
                break;
            }
        }

        // Efectueaza coliziunea dansatorului cu alt dansator
        for each (Dancer *dd in dancers)
        {
            if (d == dd) continue;  // Nu avem coliziune intre dansator si el insusi

            glm::vec3 box2MinPos{ dd->pos - dd->size / 2.0f };
            glm::vec3 box2MaxPos{ dd->pos + dd->size / 2.0f };
            if (Collision3::AABBAABB(box1MinPos, box1MaxPos, box2MinPos, box2MaxPos)) 
            {
                // Actioneaza ca o minge ce loveste alta minge
                dd->xOzDirection[0] *= -1;
                dd->xOzDirection[1] *= -1;

                d->xOzDirection[0] *= -1;
                d->xOzDirection[1] *= -1;
                movement *= -1;
                break;
            }
        }

        // Aplicam miscarea calculata
        d->Move(movement);

        // Verific si actualizez pozitia curenta in matricea de vizualizare de sus in jos
        World_object *currentTile = danceFloor.at(d->currentTopDownViewMatrixPosition);

        // Matricea podelei de dans va avea pozitii unde vezi "y" indexat astfel:
        // y00 y01 y02 y03 y04 y05 y06 y07 |  0 ---  7
        // y10 y11 y12 y13 y14 y15 y16 y17 |  8 --- 15
        // y20 y21 y22 y23 y24 y25 y26 y27 | 16 --- 23
        // y30 y31 y32 y33 y34 y35 y36 y37 | 24 --- 31
        // y40 y41 y42 y43 y44 y45 y46 y47 | 32 --- 39
        // y50 y51 y52 y53 y54 y55 y56 y57 | 40 --- 47
        // y60 y61 y62 y63 y64 y65 y66 y67 | 48 --- 55
        // y70 y71 y72 y73 y74 y75 y76 y77 | 56 --- 63

        // Cat de departe va merge dansatorul pana cand celula principala curenta pentru lumina va fi schimbata
        // De exemplu, daca setam la 0.5f atunci de indata ce centrul dansatorului trece de granita celulei de dans, se va schimba celula curenta.
        float tolerance = 0.55f;

        if (currentTile->pos.x - d->pos.x > currentTile->size.x * tolerance)
        {  
            // Mutat la celula din stanga
            if (d->currentTopDownViewMatrixPosition > 0)
            {
                d->currentTopDownViewMatrixPosition -= 1;
            }
        }
        else if (currentTile->pos.x - d->pos.x < -currentTile->size.x * tolerance)
        {  
            // Mutat la celula din dreapta
            if (d->currentTopDownViewMatrixPosition < danceFloorSize * danceFloorSize - 1)
            {
                d->currentTopDownViewMatrixPosition += 1;
            }
        }
        else if (currentTile->pos.z - d->pos.z > currentTile->size.z * tolerance)
        {  
            // Mutat la celula de sus
            if (d->currentTopDownViewMatrixPosition >= danceFloorSize)
            {
                d->currentTopDownViewMatrixPosition -= danceFloorSize;
            }
        }
        else if (currentTile->pos.z - d->pos.z < -currentTile->size.z * tolerance)
        {  
            // Mutat la celula de jos
            if (d->currentTopDownViewMatrixPosition < danceFloorSize * danceFloorSize - danceFloorSize)
            {
                d->currentTopDownViewMatrixPosition += danceFloorSize;
            }
        }
    }
}

// Actualizare surse de lumina
void Level3::UpdateSpotlights(float deltaTimeSeconds)
{
    for each (World_light *s in spotlights)
    {
        if (s->MustTurn())
        {
            s->ChangeDirection();
            s->ResetTurnTimer();
        }
        s->UpdateTurnTimer(deltaTimeSeconds);
        s->Move(deltaTimeSeconds);
    }
}
