#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"

#include "level3.h"


namespace m1
{
    class Game3 : public gfxc::SimpleScene
    {
    public:
        Game3();
        ~Game3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        // Folosite doar pentru a randa globul disco
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, std::vector<World_light*> world_lights, const glm::mat4& modelMatrix, Texture2D* texture, bool textureMoving = false);

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, std::vector<World_light*> world_lights, const glm::mat4& modelMatrix, const World_object::Material& material);
        void SetShaderLightProperties(Shader* shader, std::vector<World_light*> world_lights, const World_object::Material& material);

        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
        Mesh* CreateCone(std::string name);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderDiscoBall();
        void RenderDanceFloor();
        void RenderDiscoCeiling();
        void RenderDanceFloorWalls();
        void RenderDancers();
        void RenderSpotlights();

        std::unordered_map<std::string, Texture2D*> mapTextures;
        Level3* level;

        bool discoLightActive;
        bool floorLightActive;
        bool spotlightActive;
        bool stopDancers;
    };
}