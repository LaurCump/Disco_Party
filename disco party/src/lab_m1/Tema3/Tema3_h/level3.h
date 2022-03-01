#pragma once

#include "components/simple_scene.h"

#include "world_object.h"
#include "world_light.h"
#include "dancer.h"

namespace m1
{
    class Level3
    {
    public:
        Level3();
        ~Level3();

        void CreateDiscoBall();
        void CreateDanceFloor();
        void CreateFloorWalls();
        void CreateDiscoCeiling();
        void CreateDancers();
        void CreateSpotlights();

        void UpdateDancers(float deltaTimeSeconds);
        void UpdateSpotlights(float deltaTimeSeconds);

    public:
        unsigned int danceFloorSize;

        World_light* discoBall;
        std::vector<World_light*> spotlights;

        // Matricea podelei de dans va avea pozitii unde vezi "y" indexat astfel:
        // y00 y01 y02 y03 y04 y05 y06 y07 |  0 ---  7
        // y10 y11 y12 y13 y14 y15 y16 y17 |  8 --- 15
        // y20 y21 y22 y23 y24 y25 y26 y27 | 16 --- 23
        // y30 y31 y32 y33 y34 y35 y36 y37 | 24 --- 31
        // y40 y41 y42 y43 y44 y45 y46 y47 | 32 --- 39
        // y50 y51 y52 y53 y54 y55 y56 y57 | 40 --- 47
        // y60 y61 y62 y63 y64 y65 y66 y67 | 48 --- 55
        // y70 y71 y72 y73 y74 y75 y76 y77 | 56 --- 63

        std::vector<World_light*> danceFloor;
        std::vector<World_object*> danceFloorWalls;

        World_object* discoCeiling;
        std::vector<Dancer*> dancers;
    };
}
