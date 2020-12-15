#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    entityType;

    collideWith = nullptr;
    lastCollision;
    textureID;
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (!other->isActive) {
        return false;
    }
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    if (xdist < 0 && ydist < 0) {
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (CheckCollision(object)) {
            collideWith = object;
            lastCollision = object->entityType;
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (penetrationY < 0.9 && penetrationY > 0.2) {
                penetrationY = .05;
            }
            if (velocity.y >= 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collideTop = true;
            }
            else if (velocity.y <= 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collideEnemy = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (CheckCollision(object)) {
            collideWith = object;
            lastCollision = object->entityType;
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                velocity.x = 0;
                position.x -= penetrationX;
                collideRight = true;
            }
            else if (velocity.x < 0) {
                velocity.x = 0;
                position.x += penetrationX;
                collideLeft = true;
            }
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    lastCollision = PLATFORM;
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collideTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collideTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collideTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collideBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collideBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collideBottom = true;
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    lastCollision = PLATFORM;

    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collideLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collideRight = true;
    }
}


void Entity::AIPATROLLER() {
    switch (aiState) {
    case WALK:
        if (moved > moveLimit) {
            movement = glm::vec3(-1, 0, 0);
        }
        else if (moved < -moveLimit) {
            movement = glm::vec3(1, 0, 0);
        }
        break;
    }
}

void Entity::AICHASER(Entity* player) {
    switch (aiState) {
    case PATROL:
        if (glm::distance(position, player->position) < 3.0f) {
            aiState = WALK;
        }
        else {
            if (moved > moveLimit) {
                movement = glm::vec3(-1, 0, 0);
            }
            else if (moved < -moveLimit) {
                movement = glm::vec3(1, 0, 0);
            }
        }
        break;

    case WALK:
        if (glm::distance(position, player->position) > 2.5f) {
            aiState = PATROL;
        }
        else {
            if (player->position.x < position.x) {
                movement = glm::vec3(-2, 0, 0);
            }
            if (player->position.x > position.x) {
                movement = glm::vec3(2, 0, 0);
            }
        }
        break;
    }
}

void Entity::AI(Entity* player) {
    switch (aiType) {
    case PATROLLER:
        AIPATROLLER();
        break;
    case CHASER:
        AICHASER(player);
        break;
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map) {
    if (isActive == false) return;

    collideTop = false;
    collideBottom = false;
    collideLeft = false;
    collideRight = false;
    collideEnemy = false;
    lastCollision = PLAYER;
    gotHit = false;

    if (entityType == ENEMY) {
        moved += movement.x;
        AI(player);
    }

    if (jump) {
        jump = false;
        inAir = true;
        velocity.y += jpower;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime;
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount);

    position.x += velocity.x * deltaTime;
    CheckCollisionsX(objects, objectCount);

    if ((collideTop || collideLeft || collideRight) && lastCollision == ENEMY) {
        gotHit = true;
    }
    if (collideEnemy) {
        collideWith->isActive = false;
    }
    if (collideBottom == false) {
        inAir = true;
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}



void Entity::Render(ShaderProgram* program) {
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);


    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
