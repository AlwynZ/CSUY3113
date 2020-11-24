#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Entity.h"
#include "map.h"
#include "Util.h"
#include "Scene.h"
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Gameover.h"
#include "WinScreen.h"
#include <vector>

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool pause = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[7];

int lives = 0;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
    viewMatrix = glm::mat4(1.0f);
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("P5: Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Gameover();
    sceneList[5] = new WinScreen();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                break;

            case SDLK_RIGHT:
                break;

            case SDLK_SPACE:
                if (currentScene->state.player->collideBottom) {
                    currentScene->state.player->jump = true;
                }
                break;
            }
        case SDLK_RETURN:
            if (currentScene == sceneList[0]) {
                currentScene->state.enter = true;
            }
            break;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    if (currentScene == sceneList[4]) return;

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    if (currentScene->state.player->gotHit) {
        lives += 1;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 14.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else if (currentScene->state.player->position.x >= 14.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-14.5, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    if (lives >= 3) {
        currentScene->state.nextScene = 4;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);

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
        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}