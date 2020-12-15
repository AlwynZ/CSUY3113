#include "Menu.h"

#define Menu_WIDTH 14
#define Menu_HEIGHT 8

Mix_Music* music;

unsigned int Menu_data[] =
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

void Menu::Initialize() {

    state.nextScene = -1;

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->movement = glm::vec3(0);
    state.player->lives = 3;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(Menu_WIDTH, Menu_HEIGHT, Menu_data, mapTextureID, 1.0f, 4, 1);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("bgm.mp3");
    Mix_Volume(1, MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(music, -1);

    state.enter = false;
}

void Menu::Update(float deltaTime) {
    if (state.enter) {
        state.nextScene = 1;
    }
}
void Menu::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Dragon P6", 1, -0.4, glm::vec3(2.5, -2, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press enter to start", 0.8f, -0.5, glm::vec3(2, -4, 0));
}