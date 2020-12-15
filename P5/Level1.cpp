#include "Level1.h"
#include <SDL_mixer.h>
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 1


Mix_Chunk* sfx;

unsigned int level1_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

void Level1::Initialize() {

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3, -4, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -8.81f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("ratPlayer.png");
    state.player->isActive = true;
    state.player->lives = 0;
    state.player->gotHit = false;

    state.player->height = 1.0f;
    state.player->width = 0.5f;

    state.player->jpower = 5.0f;

    state.enemy = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("goomba.png");

    state.enemy[0].textureID = enemyTextureID;
    state.enemy[0].position = glm::vec3(16, -4, 0);
    state.enemy[0].entityType = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -4.5f, 0);
    state.enemy[0].speed = 1;
    state.enemy[0].movement = glm::vec3(1, 0, 0);
    state.enemy[0].aiType = CHASER;
    state.enemy[0].aiState = WALK;
    state.enemy[0].height = 1.0f;
    state.enemy[0].width = 0.6f;

    sfx = Mix_LoadWAV("bounce.wav");
   
}

void Level1::Update(float deltaTime) {
    state.player->lives = 0;
    state.player->Update(deltaTime, state.player, state.enemy, LEVEL1_ENEMY_COUNT, state.map);
    if (state.player->inAir && state.player->collideBottom) {
        Mix_PlayChannel(-1, sfx, 0);
        state.player->inAir = false;
    }

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        if (state.enemy[i].isActive) {
            state.enemy[i].Update(deltaTime, state.player, nullptr, 0, state.map);
        }
    }

    if (state.player->position.x >= 19) {
        state.nextScene = 2;
    }
    if (state.player->position.y <= -8) {
        state.player->gotHit = true;
    }
    if (state.player->gotHit) {
        state.player->lives += 1;
        state.player->position = glm::vec3(1, -4, 0);
    }
}
void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        if (state.enemy[i].isActive) {
            state.enemy[i].Render(program);
        }
    }
}