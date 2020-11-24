
#include "Level3.h"
#define LEVEL3_WIDTH 20
#define LEVEL3_HEIGHT 8

#define LEVEL3_AI_COUNT 3

Mix_Chunk* sfx3;

unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
 3, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 3, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -3, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -8.5f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("ratPlayer.png");
    state.player->isActive = true;
    state.player->lives = 3;

    state.player->height = 0.8f;
    state.player->width = 0.6f;

    state.player->jpower = 5.0f;

    state.enemy = new Entity[LEVEL3_AI_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("goomba.png");

    state.enemy[0].textureID = enemyTextureID;
    state.enemy[0].position = glm::vec3(10, -4, 0);
    state.enemy[0].entityType = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -4.5f, 0);
    state.enemy[0].speed = 1;
    state.enemy[0].movement = glm::vec3(1, 0, 0);
    state.enemy[0].aiType = PATROLLER;
    state.enemy[0].aiState = PATROL;
    state.enemy[0].height = 1.0f;
    state.enemy[0].width = 0.6f;
    state.enemy[0].moveLimit = 10;

    state.enemy[1].textureID = enemyTextureID;
    state.enemy[1].position = glm::vec3(10, -4, 0);
    state.enemy[1].acceleration = glm::vec3(0, -4.5f, 0);
    state.enemy[1].entityType = ENEMY;
    state.enemy[1].speed = 1;
    state.enemy[1].movement = glm::vec3(1, 0, 0);
    state.enemy[1].aiType = CHASER;
    state.enemy[1].aiState = PATROL;
    state.enemy[1].height = 1.0f;
    state.enemy[1].width = 0.6f;

    state.enemy[2].textureID = enemyTextureID;
    state.enemy[2].position = glm::vec3(15, -4, 0);
    state.enemy[2].acceleration = glm::vec3(0, -4.5f, 0);
    state.enemy[2].entityType = ENEMY;
    state.enemy[2].speed = 1;
    state.enemy[2].movement = glm::vec3(-1, 0, 0);
    state.enemy[2].aiType = PATROLLER;
    state.enemy[2].aiState = WALK;
    state.enemy[2].height = 1.0f;
    state.enemy[2].width = 0.6f;
    state.enemy[2].moveLimit = 30;



    sfx3 = Mix_LoadWAV("bounce.wav");
    Mix_Volume(1, MIX_MAX_VOLUME / 2);
}

void Level3::Update(float deltaTime) {
    state.player->lives = 0;
    state.player->Update(deltaTime, state.player, state.enemy, LEVEL3_AI_COUNT, state.map);
    if (state.player->inAir && state.player->collideBottom) {
        Mix_PlayChannel(-1, sfx3, 0);
        state.player->inAir = false;
    }

    for (int i = 0; i < LEVEL3_AI_COUNT; i++) {
        if (state.enemy[i].isActive) {
            state.enemy[i].Update(deltaTime, state.player, nullptr, 0, state.map);
        }
    }

    if (state.player->position.x >= 19) {
        state.nextScene = 5;
    }
    if (state.player->position.y <= -8) {
        state.player->gotHit = true;
    }

    if (state.player->gotHit) {
        state.player->lives += 1;
        state.player->position = glm::vec3(1, -3, 0);
    }

}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_AI_COUNT; i++) {
        if (state.enemy[i].isActive) {
            state.enemy[i].Render(program);
        }
    }
}