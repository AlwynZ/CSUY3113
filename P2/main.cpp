//2 player pong 
//Controls  ... P1: W, S       P2: Up arrow, Down Arrow

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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, p1Matrix, p2Matrix, ballMatrix, projectionMatrix;

float paddleHeight = 1.3f;
float paddleWidth = 0.45f;
float ballRadius = 0.20f; 

glm::vec3 p1Pos = glm::vec3(-5.0f, 0, 0);
glm::vec3 p2Pos = glm::vec3(5.0f, 0, 0);

glm::vec3 p1Move = glm::vec3(0, 0, 0);
glm::vec3 p2Move = glm::vec3(0, 0, 0);

float paddleSpeed = 2.0f;

glm::vec3 ballPos = glm::vec3(0, 0, 0);
glm::vec3 ballMov = glm::vec3(1, 1, 0);
float ballSpeed = 2.0f;


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 2: Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);

    projectionMatrix = glm::ortho(-5.0f, 5.0f, -4.0f, 4.0f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void ProcessInput() {

    p1Move = glm::vec3();
    p2Move = glm::vec3();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W]) {
        p1Move.y += 2;
    }
    if (keys[SDL_SCANCODE_S]) {
        p1Move.y += -2;
    }
    if (keys[SDL_SCANCODE_UP]) {
        p2Move.y += 2;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        p2Move.y += -2;
    }
}

float lastTicks = 0.0f;
float acceleration = 0.0f;    //increase both paddle speed and ball speed every time the ball hits a paddle
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    if (fabs(ballPos.x) > 5.0f - ballRadius) {
        return;
    }

    p1Pos += p1Move * (paddleSpeed + acceleration) * deltaTime;  
    p2Pos += p2Move * (paddleSpeed + acceleration) * deltaTime;
    ballPos += ballMov * (ballSpeed + acceleration) * deltaTime;

    p1Pos.y = fmin(p1Pos.y, 4.0f - paddleHeight / 2); 
    p1Pos.y = fmax(p1Pos.y, -4.0f + paddleHeight / 2);

    p2Pos.y = fmin(p2Pos.y, 4.0f - paddleHeight / 2); 
    p2Pos.y = fmax(p2Pos.y, -4.0f + paddleHeight / 2);

    ballPos.y = fmin(ballPos.y, 4.0f - ballRadius);
    ballPos.y = fmax(ballPos.y, -4.0f + ballRadius);

    //floor and ceiling collision
    if (fabs(ballPos.y) >= 4.0f - ballRadius) {
        ballMov.y *= -1;
    }

    if (fabs(p1Pos.x - ballPos.x) - (paddleWidth)/2 - ballRadius < 0 && (fabs(p1Pos.y - ballPos.y) - (paddleHeight)/2 - ballRadius < 0)) {
        ballMov.x = 1.0f;
        acceleration = acceleration + 0.1f;        //increase speed
    }
    if (fabs(p2Pos.x - ballPos.x) - (paddleWidth)/2 - ballRadius < 0 && (fabs(p2Pos.y - ballPos.y) - (paddleHeight)/2 - ballRadius < 0)) {
        ballMov.x = -1.0f;
        acceleration = acceleration + 0.1f;       //increase speed
    }

    p1Matrix = glm::mat4(1.0f);
    p1Matrix = glm::translate(p1Matrix, p1Pos);
    p2Matrix = glm::mat4(1.0f);
    p2Matrix = glm::translate(p2Matrix, p2Pos);
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ballPos);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        -0.5 * paddleWidth, -0.5 * paddleHeight,
        0.5 * paddleWidth, -0.5 * paddleHeight,
        0.5*paddleWidth, 0.5 * paddleHeight,
        -0.5 * paddleWidth, -0.5 * paddleHeight,
        0.5 * paddleWidth, 0.5 * paddleHeight,
        -0.5 * paddleWidth, 0.5 * paddleHeight };

    program.SetModelMatrix(p1Matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(p2Matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    float ball_vertices[] = {
        -1 * ballRadius, -1 * ballRadius,
        1 * ballRadius, -1 * ballRadius,
        1 * ballRadius, 1 * ballRadius,
        -1 * ballRadius, -1 * ballRadius,
        1 * ballRadius, 1 * ballRadius,
        -1 * ballRadius, 1 * ballRadius };

    program.SetModelMatrix(ballMatrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball_vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);


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