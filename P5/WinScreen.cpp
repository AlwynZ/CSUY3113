#include "WinScreen.h"

#define WinScreen_WIDTH 14
#define WinScreen_HEIGHT 8

unsigned int Win_data[] =
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void WinScreen::Initialize() {

    state.nextScene = -1;

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, 0, 0);
    state.player->isActive = false;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(WinScreen_WIDTH, WinScreen_HEIGHT, Win_data, mapTextureID, 1.0f, 4, 1);

}

void WinScreen::Update(float deltaTime) {

}
void WinScreen::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "You Win!", 1, -0.5, glm::vec3(3.5, -2, 0));
}