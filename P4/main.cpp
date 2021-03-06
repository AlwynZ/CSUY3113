/* W to jump
    A to move left
    D to move right

    jump on top of enemies to kill them

    enemy behaviour from bottom to top: 
    approaches player and jumps at player when close 
    moves back and forth on its platform
    chases player at higher speed when within range 
*/

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include <vector>


struct GameState {
    Entity* player;
    Entity* enemy;
    Entity* platform;

};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool pause = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);

}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("P4: Rise of the AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4.8f, -2.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -4.25f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("ratPlayer.png");

    state.player->height = 0.8f;
    state.player->width = 0.65f;

    state.player->jpower = 5.0f;



    state.platform = new Entity[23];
    GLuint platformTextureID = LoadTexture("platform.png");

    for (int i = 0; i < 15; i++) {
        state.platform[i].textureID = platformTextureID;
        state.platform[i].position = glm::vec3(-5 + i, -3.25f, 0);
        state.platform[i].entityType = PLATFORM;
    }
     
    for (int i = 15; i < 18; i++) {
        state.platform[i].textureID = platformTextureID;
        state.platform[i].position = glm::vec3(-13 + i, -1.0f, 0);
        state.platform[i].entityType = PLATFORM;
    }
    

    for (int i = 18; i < 23; i++) {
        state.platform[i].textureID = platformTextureID;
        state.platform[i].position = glm::vec3(-22 + i, 1.0f, 0);
        state.platform[i].entityType = PLATFORM;
    }


    for (int i = 0; i < 23; i++) {
        state.platform[i].Update(0, NULL, NULL, 0);
    }

    state.enemy = new Entity[3];
    GLuint enemyTextureID = LoadTexture("goomba.png");
    
    state.enemy[0].textureID = enemyTextureID;
    state.enemy[0].position = glm::vec3(-2, 2.0f, 0);
    state.enemy[0].entityType = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -4.5f, 0);
    state.enemy[0].speed = 1;
    state.enemy[0].movement = glm::vec3(1, 0, 0);
    state.enemy[0].aiType = CHASER;
    state.enemy[0].aiState = PATROL;
    state.enemy[0].height = 1.0f;
    state.enemy[0].width = 0.6f;
   
    state.enemy[1].textureID = enemyTextureID;
    state.enemy[1].position = glm::vec3(2, 0.0f, 0);
    state.enemy[1].entityType = ENEMY;
    state.enemy[1].speed = 1;
    state.enemy[1].movement = glm::vec3(-1, 0, 0);
    state.enemy[1].aiType = PATROLLER;
    state.enemy[1].aiState = WALK;
    state.enemy[1].height = 0.8f;
    state.enemy[1].width = 0.6f;

    state.enemy[2].textureID = enemyTextureID;
    state.enemy[2].position = glm::vec3(4, -2.25f, 0);
    state.enemy[2].entityType = ENEMY;
    state.enemy[2].speed = 1;
    state.enemy[2].acceleration = glm::vec3(0, -4.5f, 0);
    state.enemy[2].movement = glm::vec3(1, 0, 0);
    state.enemy[2].aiType = JUMPER;
    state.enemy[2].aiState = FOLLOW;
    state.enemy[2].height = 0.8f;
    state.enemy[2].width = 0.6f;
    state.enemy[2].jpower = 1.5f;

}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_a:
                break;

            case SDLK_d:
                break;

            case SDLK_w:
                if (state.player->collideBottom) {
                    state.player->jump = true;
                }
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_A]) {
        state.player->movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        state.player->movement.x = 1.0f;
    }

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    if (state.player->isActive == false) return;
    if (pause) return;

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP, state.player, state.enemy, 3);
        state.player->Update(FIXED_TIMESTEP, state.player, state.platform, 30);

        for (int i = 0; i < 3; i++) {
            state.enemy[i].Update(FIXED_TIMESTEP, state.player, state.platform, 30);
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < 30; i++) {
        state.platform[i].Render(&program);
    }
    bool over = true;
    for (int i = 0; i < 3; i++) {
        state.enemy[i].Render(&program);
        if (state.enemy[i].isActive == true) {
            over = false;
        }
    }

    state.player->Render(&program);

    if (state.player->isActive == false) {
        DrawText(&program, LoadTexture("font1.png"), "GAME OVER", 1, -0.5f, glm::vec3(-3.3, 1.5, 0));
    }
    else if (over) {
        DrawText(&program, LoadTexture("font1.png"), "YOU WIN", 1, -0.5f, glm::vec3(-3.3, 1.5, 0));
        pause = true;
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}