#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <glm/glm.hpp>

#include "../ecs/ecs.h"
#include "../components/transformComponent.h"
#include "../components/rigidBodyComponent.h"
#include "../components/spriteComponent.h"
#include "../components/boxColliderComponent.h"
#include "../components/projectileEmitterComponent.h"
#include "../components/healthComponent.h"

/*
 * This system is for rendering all the things related to a ImGUI for creating enemies
*/
class RenderGUISystem: public System {
    public:
        RenderGUISystem() = default;
        void update(SDL_Renderer* renderer, const std::unique_ptr<Registry>& registry, const SDL_Rect& camera) {
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            
            // create all the ImGUI elements
            if (ImGui::Begin("Spawn enemies")) {
                // Variables for the enemy entity
                static int enemyXPos = 0;
                static int enemyYPos = 0;
                static int scaleX = 1;
                static int scaleY = 1;
                static int velX = 0;
                static int velY = 0;
                static int health = 0;
                static float rotation = 0.0;
                static float projAngle = 0.0;
                static float projSpeed = 100.0;
                static int projRepeat = 10;
                static int projDuration = 10;
                const char* sprites[] = {"tank-image", "truck-image"};
                static int selectedSpriteIndex = 0;

                if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Combo("Texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("Position X", &enemyXPos);
                    ImGui::InputInt("Position Y", &enemyYPos);
                    ImGui::SliderInt("scale X", &scaleX, 1, 10);
                    ImGui::SliderInt("Scale Y", &scaleY, 1, 10);
                    ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("Velocity X", &velX);
                    ImGui::InputInt("Velocity Y", &velY);
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
                    ImGui::SliderFloat("speed (Px/sec)", &projSpeed, 10, 500);
                    ImGui::InputInt("repeat (sec)", &projRepeat);
                    ImGui::InputInt("duration (sec)", &projDuration);
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderInt("Initial health %", &health, 1, 100);

                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                
                
                // button to generate the enemy entity
                if (ImGui::Button("Create new enemy")) {
                    // button clicked!
                    Entity enemy = registry->createEntity();
                    enemy.group("enemies");
                    enemy.addComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                    enemy.addComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                    enemy.addComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32,32, 2);
                    enemy.addComponent<BoxColliderComponent>(25, 20, glm::vec2(5,5));
                    double projVelX = cos(projAngle) * projSpeed;
                    double projVelY = sin(projAngle) * projSpeed;
                    enemy.addComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false);
                    enemy.addComponent<HealthComponent>(health);

                    // reset all values after creating the entity
                    enemyXPos = enemyYPos = rotation = projAngle = 0;
                    scaleX = scaleY = 1;
                    projRepeat = projDuration = 10;
                    projSpeed = 100;
                    health = 10;
                }
            }
            ImGui::End();

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0,0));
            ImGui::SetNextWindowBgAlpha(0.9f);
            if (ImGui::Begin("Map Coordinates", NULL, windowFlags)) {
                ImGui::Text(
                    "Map Coordinates(x=%.1f, y=%.1f)",
                    ImGui::GetIO().MousePos.x + camera.x,
                    ImGui::GetIO().MousePos.y + camera.y
                );
            }
            ImGui::End();
            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        }
};

#endif