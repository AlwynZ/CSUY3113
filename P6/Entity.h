#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "map.h"

enum EntityType { PLAYER, BLOCK, PROJECTILE };

enum AIType { HORIZONTAL, VERTICAL };
enum AIState { MOVE, CHASE, STOP };

class Entity {
public:

    EntityType entityType;
    EntityType lastCollision;
    Entity* collideWith;

    AIType aiType;
    AIState aiState;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;

    float width = 1;
    float height = 1;

    bool flap = false;
    bool inAir = true;
    float jpower = 0;

    int lives = 3;

    bool gotHit = false;

    GLuint textureID;

    glm::mat4 modelMatrix;

    bool isActive = true;
    bool collided = false;
    bool collideTop = false;
    bool collideBottom = false;
    bool collideLeft = false;
    bool collideRight = false;
    bool collideEnemy = false;

    int moved = 0;
    int moveLimit = 0;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsY(Map* map);
    void CheckCollisionsX(Map* map);

    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram* program);

    void AI(Entity* player);
    //void AIHOMING();
    void AIHORIZONTAL();
    void AIVERTICAL();

};
