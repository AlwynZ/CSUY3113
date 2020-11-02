#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    entityType;
    lastCollision;
    collideWith = nullptr;
    inAir = false;

    isActive = true;

    textureID;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xDistance = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float yDistance = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xDistance < 0 && yDistance < 0) {
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (object->entityType == ENEMY) return;
        if (CheckCollision(object)) {
            float yDistance = fabs(position.y - object->position.y);
            float yPenetration = fabs(yDistance - (height / 2.0f) - (object->height / 2.0f));
            if (yPenetration < 1.0 && yPenetration > 0.2) {
                yPenetration = .05;
            }
            if (velocity.y > 0) {
                position.y -= yPenetration;
                velocity.y = 0;
            }
            else if (velocity.y < 0) {
                position.y += yPenetration;
                velocity.y = 0;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (object->entityType == ENEMY) return;
        if (CheckCollision(object)) {
            float xDistance = fabs(position.x - object->position.x);
            float xPenetration = fabs(xDistance - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= xPenetration;
                velocity.x = 0;
            }
            else if (velocity.x < 0) {
                position.x += xPenetration;
                velocity.x = 0;
            }
        }
    }
}

void Entity::CheckEnemyY(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (CheckCollision(object)) {
            collideWith = object;
            if (position.y > object->position.y) {
                collideBottom = true;
                lastCollision = object->entityType;
            }
            else if (position.y < object->position.y) {
                collideTop = true;
                lastCollision = object->entityType;
            }
        }
    }
}

void Entity::CheckEnemyX(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        if (CheckCollision(object)) {
            collideWith = object;
            if (position.x > object->position.x) {
                collideLeft = true;
                lastCollision = object->entityType;
            }
            else if (position.x < object->position.x) {
                collideRight = true;
                lastCollision = object->entityType;
            }
        }
    }
}

void Entity::AIPATROLLER() {
    switch (aiState) {
    case WALK:
        if (position.x > 3.5) {
            movement = glm::vec3(-1, 0, 0);
        }
        else if (position.x < 2) {
            movement = glm::vec3(1, 0, 0);
        }
        break;
    }
}

void Entity::AICHASER(Entity* player) {
    switch (aiState) {
    case PATROL:
        if (glm::distance(position, player->position) < 2.5f) {
            aiState = WALK;
        }
        else {
            if (position.x > 0) {
                movement = glm::vec3(-1, 0, 0);
            }
            else if (position.x < -4) {
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

void Entity::AIJUMPER(Entity* player) {
    bool jumped = false;
    switch (aiType) {
    case FOLLOW:
        
        if (glm::distance(position, player->position) < 1.5 && !inAir)  {
            velocity.y = jpower;
            jumped = true;
        }
        
        if (glm::distance(position, player->position) < 2.0f) {
            aiState = STOP;
        }
        else {
            if (player->position.x < position.x) {
                movement = glm::vec3(-1.5, 0, 0);
            }
            if (player->position.x > position.x) {
                movement = glm::vec3(1.5, 0, 0);
            }
        }
        break;
    case STOP:
        if (glm::distance(position, player->position) > 2.0f) {
            aiState = FOLLOW;
        }
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
    case JUMPER:
        AIJUMPER(player);
        break;
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* object, int objectCount) {
    if (isActive == false) return;

    collideTop = false;
    collideBottom = false;
    collideLeft = false;
    collideRight = false;
    inAir = false;

    if (entityType == ENEMY) {
        AI(player);
    }

    if (jump) {
        velocity.y += jpower;
        jump = false;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime;
    position.x += velocity.x * deltaTime;

    CheckEnemyY(object, objectCount);
    CheckEnemyX(object, objectCount);

    if ((collideBottom || collideTop) && (collideLeft || collideRight)) {
        inAir = true;
    }

    CheckCollisionsY(object, objectCount);
    CheckCollisionsX(object, objectCount);

    if ((collideLeft || collideRight) && !collideBottom && collideWith->entityType == ENEMY) {
        isActive = false;
    }

    if (collideBottom && lastCollision == ENEMY) {
        collideWith->isActive = false;
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
