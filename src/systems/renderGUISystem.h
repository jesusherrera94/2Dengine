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
        void update(SDL_Renderer* renderer, const std::unique_ptr<Registry>& registry) {
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;
            // create all the ImGUI elements
            if (ImGui::Begin("Spawn enemies")) {
                // Input for the X position
                static int enemyXPos = 0;
                static int enemyYPos = 0;
                ImGui::InputInt("Enemy X position", &enemyXPos);
                // Input for the Y position
                ImGui::InputInt("Enemy Y position", &enemyYPos);
                // button to generate the enemy entity
                if (ImGui::Button("Create new enemy")) {
                    // button clicked!
                    Entity enemy = registry->createEntity();
                    enemy.group("enemies");
                    enemy.addComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(1.0, 1.0), 0.0);
                    enemy.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
                    enemy.addComponent<SpriteComponent>("tank-image", 32,32, 2);
                    enemy.addComponent<BoxColliderComponent>(32, 32);
                    enemy.addComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
                    enemy.addComponent<HealthComponent>(100);
                }
            }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        }
};

#endif