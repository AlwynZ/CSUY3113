#include "Level1.h"
#include <SDL_mixer.h>
#define LEVEL1_WIDTH 160
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 5

Mix_Chunk* sfxHit;

unsigned int level1_data[] =
{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
};

void Level1::Initialize() {

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3.0f, 2.0f, 0);
    state.player->movement = glm::vec3(0.0f, 0.0f, 0);
    state.player->acceleration = glm::vec3(100.0f, -8.81, 0);
    state.player->velocity = glm::vec3(20.0f, 2.0f, 0);
    state.player->speed = 10.0f;
    state.player->textureID = Util::LoadTexture("dragon.png");
    state.player->isActive = true;
    state.player->lives = 0;
    state.player->gotHit = false;

    state.player->height = 1.0f;
    state.player->width = 0.5f;

    state.player->jpower = 3.0f;

    state.enemy = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("fireball.png");

    sfxHit = Mix_LoadWAV("hit.wav");


   /*
    state.enemy[0].textureID = enemyTextureID;
    state.enemy[0].position = glm::vec3(5.0f, -4, 0);
    state.enemy[0].entityType = PROJECTILE;
    state.enemy[0].acceleration = glm::vec3(0, -2.0f, 0);
    state.enemy[0].speed = 1;
    state.enemy[0].movement = glm::vec3(0, -1, 0);
    state.enemy[0].aiType = VERTICAL;
    state.enemy[0].moveLimit = 20;
    state.enemy[0].aiState = MOVE;
    state.enemy[0].height = 1.0f;
    state.enemy[0].width = 1.0f;
    state.enemy[0].isActive = true;

    
    state.enemy[1].textureID = enemyTextureID;
    state.enemy[1].position = glm::vec3(20, -4, 0);
    state.enemy[1].entityType = PROJECTILE;
    state.enemy[1].acceleration = glm::vec3(0, 0, 0);
    state.enemy[1].speed = 2;
    state.enemy[1].movement = glm::vec3(-1, 0, 0);
    state.enemy[1].aiType = HORIZONTAL;
    state.enemy[1].aiState = MOVE;
    state.enemy[1].height = 1.0;
    state.enemy[1].width = 1.0;

    
    state.enemy[2].textureID = enemyTextureID;
    state.enemy[2].position = glm::vec3(40.0f, -4, 0);
    state.enemy[2].entityType = PROJECTILE;
    state.enemy[2].acceleration = glm::vec3(0, 0, 0);
    state.enemy[2].speed = 2;
    state.enemy[2].movement = glm::vec3(-1, 0, 0);
    state.enemy[2].aiType = FIREBALL;
    state.enemy[2].aiState = MOVE;
    state.enemy[2].height = 1.0f;
    state.enemy[2].width = 1.0f;

    state.enemy[3].textureID = enemyTextureID;
    state.enemy[3].position = glm::vec3(15.0f, -3, 0);
    state.enemy[3].entityType = PROJECTILE;
    state.enemy[3].acceleration = glm::vec3(0, 0, 0);
    state.enemy[3].speed = 2;
    state.enemy[3].movement = glm::vec3(-1, 0, 0);
    state.enemy[3].aiType = FIREBALL;
    state.enemy[3].aiState = MOVE;
    state.enemy[3].height = 1.0f;
    state.enemy[3].width = 1.0f;

    state.enemy[4].textureID = enemyTextureID;
    state.enemy[4].position = glm::vec3(50, -3, 0);
    state.enemy[4].entityType = PROJECTILE;
    state.enemy[4].acceleration = glm::vec3(0, 0, 0);
    state.enemy[4].speed = 2;
    state.enemy[4].movement = glm::vec3(-1, 0, 0);
    state.enemy[4].aiType = FIREBALL;
    state.enemy[4].aiState = MOVE;
    state.enemy[4].height = 1.0f;
    state.enemy[4].width = 1.0f;

    state.enemy[5].textureID = enemyTextureID;
    state.enemy[5].position = glm::vec3(58, -3.5, 0);
    state.enemy[5].entityType = PROJECTILE;
    state.enemy[5].acceleration = glm::vec3(0, 0, 0);
    state.enemy[5].speed = 2;
    state.enemy[5].movement = glm::vec3(-1, 0, 0);
    state.enemy[5].aiType = FIREBALL;
    state.enemy[5].aiState = MOVE;
    state.enemy[5].height = 1.0f;
    state.enemy[5].width = 1.0f;

    */
   
}

void Level1::Update(float deltaTime) {
    state.player->lives = 0;
    state.player->Update(deltaTime, state.player, state.enemy, LEVEL1_ENEMY_COUNT, state.map);

    if (state.player->position.x >= 157) {
        state.nextScene = 3;
    }
    if (state.player->position.y <= -8) {
        state.player->gotHit = true;
    }
    if (state.player->gotHit) {
        state.player->lives += 1;
        Mix_PlayChannel(-1, sfxHit, 0);
        state.player->position = glm::vec3(3, -4, 0);
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