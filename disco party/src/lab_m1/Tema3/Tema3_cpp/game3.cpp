#include "lab_m1/Tema3/Tema3_h/game3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Game3::Game3()
{
}


Game3::~Game3()
{
    delete level;
}


void Game3::Init()
{
    discoLightActive = true;
    floorLightActive = true;
    spotlightActive = true;
    stopDancers = false;

    level = new Level3();

    // Load textures
    {
        mapTextures["random"] = CreateRandomTexture(16, 16);  // 16 x 16 rezolutia
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Creem forma de con pentru lumina
    {
        Mesh* mesh = CreateCone("cone");
        AddMeshToList(mesh);
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        std::string GAME_SOURCE_PATH = PATH_JOIN("src", "lab_m1", "Tema3");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Game3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Game3::Update(float deltaTimeSeconds)
{
    if (!stopDancers)
    {
        level->UpdateDancers(deltaTimeSeconds);
    }
    level->UpdateSpotlights(deltaTimeSeconds);
}


void Game3::FrameEnd()
{
    //DrawCoordinateSystem();
    RenderDiscoBall();
    RenderDancers();
    RenderDanceFloor();
    RenderDiscoCeiling();
    RenderDanceFloorWalls();
    RenderSpotlights();
}

//Folosit doar pentru a randa globul disco
void Game3::RenderSimpleMesh(Mesh *mesh, Shader *shader, std::vector<World_light*> world_lights, const glm::mat4 & modelMatrix, Texture2D* texture, bool textureMoving)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    location = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    location = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // - activate texture location 0
    glActiveTexture(GL_TEXTURE0);
    // - bind the texture1 ID
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    // - send the uniform value
    glUniform1i(glGetUniformLocation(shader->program, "disco_ball_texture"), 0);

    // Get shader location for uniform float "elapsed_time"
    location = glGetUniformLocation(shader->GetProgramID(), "elapsed_time");
    GLfloat elapsedTime{ static_cast<GLfloat>(Engine::GetElapsedTime()) };
    // Set shader uniform "elapsed_time" to elapsedTime
    glUniform1f(location, elapsedTime);

    // Get shader location for uniform "is_texture_moving" and set it to textureMoving
    glUniform1i(glGetUniformLocation(shader->program, "is_texture_moving"), textureMoving);

    // Set shader uniforms for light properties
    {
        World_object::Material material;

        // negru complet transparent(felul de a spune ca nu are culoare, are textura)
        material.color = glm::vec4(0.0f);
        material.kd = 0.5f;
        material.ks = 0.5f;
        material.shininess = 30;
        SetShaderLightProperties(shader, world_lights, material);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Game3::RenderSimpleMesh(Mesh* mesh, Shader* shader, std::vector<World_light*> world_lights, const glm::mat4& modelMatrix, const World_object::Material& material)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    location = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    location = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Get shader location for uniform float "elapsed_time"
    location = glGetUniformLocation(shader->GetProgramID(), "elapsed_time");
    GLfloat elapsedTime{ static_cast<GLfloat>(Engine::GetElapsedTime()) };
    // Set shader uniform "elapsed_time" to elapsedTime
    glUniform1f(location, elapsedTime);

    // Set shader uniforms for light properties
    SetShaderLightProperties(shader, world_lights, material);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Game3::SetShaderLightProperties(Shader* shader, std::vector<World_light*> world_lights, const World_object::Material& material)
{
    if (!shader || !shader->GetProgramID())
        return;

    glUniform1i(glGetUniformLocation(shader->program, "num_lights"), world_lights.size());

    int location;
    std::string inShaderSpotlight;
    std::string inShaderPositionName;
    std::string inShaderDirectionName;
    std::string inShaderColorName;
    int i = 0;

    for each (World_light * l in world_lights)
    {
        inShaderSpotlight = "is_spotlight[" + std::to_string(i) + ']';
        inShaderPositionName = "light_position[" + std::to_string(i) + ']';
        inShaderDirectionName = "light_direction[" + std::to_string(i) + ']';
        inShaderColorName = "light_color[" + std::to_string(i) + ']';

        location = glGetUniformLocation(shader->program, inShaderSpotlight.data());
        glUniform1i(location, l->isSpotlight);

        location = glGetUniformLocation(shader->program, inShaderColorName.data());
        glUniform3f(location, l->lightColor.r, l->lightColor.g, l->lightColor.b);

        location = glGetUniformLocation(shader->program, inShaderPositionName.data());
        glUniform3f(location, l->pos.x, l->pos.y, l->pos.z);

        location = glGetUniformLocation(shader->program, inShaderDirectionName.data());
        glUniform3f(location, l->lightDirection.x, l->lightDirection.y, l->lightDirection.z);

        ++i;
    }

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    location = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(location, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object_color) 
    location = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(location, material.shininess);
    location = glGetUniformLocation(shader->program, "material_ambient_light");
    glUniform1f(location, material.ambient_light);
    location = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(location, material.kd);
    location = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(location, material.ks);
    location = glGetUniformLocation(shader->GetProgramID(), "object_color");
    glUniform4fv(location, 1, glm::value_ptr(material.color));

    // Trimitem pozitia globului disco
    location = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(location, level->discoBall->pos.x, level->discoBall->pos.y, level->discoBall->pos.z);

    // Setam modurile de lumina
    glUniform1i(glGetUniformLocation(shader->program, "disco_light_active"), discoLightActive);
    glUniform1i(glGetUniformLocation(shader->program, "floor_light_active"), floorLightActive);
    glUniform1i(glGetUniformLocation(shader->program, "spotlight_active"), spotlightActive);
}


Texture2D* Game3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // Generate random texture data
    for (int i = 0; i < size; i += 3)
    {
        data[i] = rand() % 256;			// rosu
        data[i + 1] = rand() % 256;		// verde
        data[i + 2] = rand() % 256;		// albastru
    }

    // Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) 
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    // Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Horizontal mode wrapping

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

// Crearea conului de lumina
Mesh* Game3::CreateCone(std::string name)
{
    float height = 1;
    glm::vec3 color{ glm::vec3(0) };
    glm::vec3 center{ glm::vec3(0, height, 0) };  // Mutam centrul in sus pentru a crea conul
    float radius{ 0.5f };

    int i;
    float angle = 0;
    int points = 60;
    float angleOffset = 360.0f / points;

    std::vector<VertexFormat> vertices
    {
        VertexFormat(center, color)
    };

    for (i = 1; i <= points; ++i) 
    {
        float x = center.x + glm::cos(glm::radians(angle)) * radius;
        float y = 0;
        float z = center.z + glm::sin(glm::radians(angle)) * radius;

        VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
        vertices.push_back(v);
        angle += angleOffset;
    }

    Mesh* shape = new Mesh(name);

    std::vector<unsigned int> indices;

    for (i = 0; i <= points; ++i) 
    {
        indices.push_back(i);
    }
    indices.push_back(1);

    shape->SetDrawMode(GL_TRIANGLE_FAN);

    shape->InitFromData(vertices, indices);

    return shape;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Game3::OnInputUpdate(float deltaTime, int mods)
{
}

void Game3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_P)
    {
        stopDancers = !stopDancers;
    }
    if (key == GLFW_KEY_1)
    {
        discoLightActive = !discoLightActive;
    }
    if (key == GLFW_KEY_2)
    {
        floorLightActive = !floorLightActive;
    }
    if (key == GLFW_KEY_3)
    {
        spotlightActive = !spotlightActive;
    }
}

void Game3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Game3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void Game3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Game3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Game3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Game3::OnWindowResize(int width, int height)
{
}

// Desenam globul disco
void Game3::RenderDiscoBall()
{
    if (!discoLightActive) return;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, level->discoBall->pos);
    modelMatrix = glm::scale(modelMatrix, level->discoBall->size);
    // Rotirea trebuie sa se intample la nivelul texturii in fragment shader sau vertex shader.

    std::vector<World_light*> world_lights;

    // Globul nu va fi luminat de orice.

    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], world_lights, modelMatrix, mapTextures["random"], true);
}

// Desenam ringul de dans
void Game3::RenderDanceFloor()
{
    for each (World_object *d in level->danceFloor)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, d->pos);
        modelMatrix = glm::scale(modelMatrix, d->size);

        std::vector<World_light*> world_lights;

        // Podeaua va fi luminata de toate sursele de lumina.
        if (spotlightActive)
        {
            for each (World_light * l in level->spotlights)
            {
                world_lights.push_back(l);
            }
        }

        if (discoLightActive)
        {
            world_lights.push_back(level->discoBall);
        }

        World_object::Material mat{ d->material };
        if (!floorLightActive)
        {
            mat.ambient_light = 0.0f;
        }

        // Podeaua va fi afectata doar de sursele de lumina.

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], world_lights, modelMatrix, mat);
    }
}

// Desenam tavanul din ringul de dans
void Game3::RenderDiscoCeiling()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, level->discoCeiling->pos);
    modelMatrix = glm::scale(modelMatrix, level->discoCeiling->size);

    std::vector<World_light*> world_lights;

    // Tavanul va fi luminat doar de lumina care vine de la glob.
    if (discoLightActive)
    {
        world_lights.push_back(level->discoBall);
    }

    RenderSimpleMesh(meshes["box"], shaders["LabShader"], world_lights, modelMatrix, level->discoCeiling->material);
}

// Desenam zidurile
void Game3::RenderDanceFloorWalls()
{
    int i = 0;
    for each (World_object * d in level->danceFloorWalls)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, d->pos);
        modelMatrix = glm::scale(modelMatrix, d->size);

        std::vector<World_light*> world_lights;

        if (spotlightActive)
        {
            for each (World_light *l in level->spotlights)
            {
                world_lights.push_back(l);
            }
        }

        // Matricea podelei de dans va avea pozitii unde vezi "y" indexat astfel:
        // y00 y01 y02 y03 y04 y05 y06 y07 |  0 ---  7
        // y10 y11 y12 y13 y14 y15 y16 y17 |  8 --- 15
        // y20 y21 y22 y23 y24 y25 y26 y27 | 16 --- 23
        // y30 y31 y32 y33 y34 y35 y36 y37 | 24 --- 31
        // y40 y41 y42 y43 y44 y45 y46 y47 | 32 --- 39
        // y50 y51 y52 y53 y54 y55 y56 y57 | 40 --- 47
        // y60 y61 y62 y63 y64 y65 y66 y67 | 48 --- 55
        // y70 y71 y72 y73 y74 y75 y76 y77 | 56 --- 63

        if (floorLightActive)
        {
            int num_tiles = level->danceFloorSize * level->danceFloorSize;
            if (i == 0)  // Peretele de sus cu vederea de sus in jos
            {
                for (int j = 0; j < level->danceFloorSize; ++j)
                {
                    world_lights.push_back(level->danceFloor.at(j));
                }
            }
            else if (i == 1)  // Peretele de jos cu vederea de sus in jos
            {
                for (int j = num_tiles - level->danceFloorSize; j < num_tiles; ++j)
                {
                    world_lights.push_back(level->danceFloor.at(j));
                }
            }
            else if (i == 2)  // Peretele din stanga cu vederea de sus in jos
            {
                for (int j = 0; j <= num_tiles - level->danceFloorSize; j += 8)
                {
                    world_lights.push_back(level->danceFloor.at(j));
                }
            }
            else if (i == 3)  // Peretele din dreapta cu vederea de sus in jos
            {
                for (int j = 7; j < num_tiles; j += 8)
                {
                    world_lights.push_back(level->danceFloor.at(j));
                }
            }
        }

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], world_lights, modelMatrix, d->material);
        ++i;
    }
}

// Desenam dansatorii
void Game3::RenderDancers()
{
    for each (Dancer *d in level->dancers)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, d->pos);
        modelMatrix = glm::scale(modelMatrix, d->size);

        std::vector<World_light*> world_lights;

        // Dansatorul va fi luminat de toate sursele de lumina.
        if (spotlightActive)
        {
            for each (World_light *l in level->spotlights)
            {
                world_lights.push_back(l);
            }
        }

        if (floorLightActive)
        {
            // Un dansator va fi luminat de cele mai aproape 9 celule ale podelei.

            // Celula din centru
            world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition));

            if (d->currentTopDownViewMatrixPosition > 0)
            {  
                // Celula din stanga
                world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition - 1));
                if (d->currentTopDownViewMatrixPosition - 1 >= level->danceFloorSize)
                {  
                    // Celula din stanga sus
                    world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition - 1 - level->danceFloorSize));
                }
                if (d->currentTopDownViewMatrixPosition - 1 < level->danceFloorSize * level->danceFloorSize - level->danceFloorSize)
                {  
                    // Celula din stanga jos
                    world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition - 1 + level->danceFloorSize));
                }
            }
            if (d->currentTopDownViewMatrixPosition < level->danceFloorSize * level->danceFloorSize - 1)
            {  
                // Celula din dreapta
                world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition + 1));
                if (d->currentTopDownViewMatrixPosition + 1 >= level->danceFloorSize)
                {  
                    // Celula din dreapta sus
                    world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition + 1 - level->danceFloorSize));
                }
                if (d->currentTopDownViewMatrixPosition + 1 < level->danceFloorSize * level->danceFloorSize - level->danceFloorSize)
                {  
                    // Celula din dreapta jos
                    world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition + 1 + level->danceFloorSize));
                }
            }
            if (d->currentTopDownViewMatrixPosition >= level->danceFloorSize)
            {  
                // Celula de sus
                world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition - level->danceFloorSize));
            }
            if (d->currentTopDownViewMatrixPosition < level->danceFloorSize * level->danceFloorSize - level->danceFloorSize)
            {  
                // Celula de jos
                world_lights.push_back(level->danceFloor.at(d->currentTopDownViewMatrixPosition + level->danceFloorSize));
            }
        }

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], world_lights, modelMatrix, d->material);
    }
}

// Desenam sursele de lumina
void Game3::RenderSpotlights()
{
    if (!spotlightActive) return;

    // Doar partea din fata a patratelor vor fi desenate.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Aceasta este folosita pentru a opri scrierea in depth buffer.
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for each (World_light* s in level->spotlights)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 pos{ s->pos };
        pos.y = s->size.y - 1;  // Avand conuri cu o unitate mai mare astfel incat atunci cand se roteste, tot va atinge ringul de dans
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::rotate(modelMatrix, s->oxAngleRadians, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, s->ozAngleRadians, glm::vec3(0, 0, 1));
        pos = glm::vec3(0, -s->size.y, 0);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::scale(modelMatrix, s->size);

        std::vector<World_light*> world_lights;
        // Sursele de lumina nu vor fi luminate de orice.

        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], world_lights, modelMatrix, s->material);
    }

    // Dezactivam actiunile numite anterior
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}
