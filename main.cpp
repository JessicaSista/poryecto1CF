#include <windows.h> 
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "game.h"

#include "Camera.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Worm Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Set viewport y proyecci�n
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);  // Color de fondo

    Game game;
    Camera camera;  // Creamos un objeto de c�mara
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                std::cout << "Key down: " << SDL_GetScancodeName(event.key.keysym.scancode) << std::endl;

                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && game.getState() == MENU) {
                    std::cout << "ENTER pressed, changing state to PLAYING" << std::endl;
                    game.setState(PLAYING);
                    camera.apply(15 / 2.f, -15/2.f + 3, 0);
                    std::cout << "state: PLAYING" << std::endl;
                }
                game.handleInput(event.key.keysym.sym);
            }

            camera.update(event);  // Actualizamos la c�mara con los eventos
        }
        game.update();

        if (game.getState() == MENU) {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

           // camera.apply(0.f, 0, 0);  // Aplicamos la transformaci�n de la c�mara (mirando al origen de la escena)

            game.renderMenu();
        }
        else if (game.getState() == WON)
            game.renderWinScreen();
        else {
            game.render();
        }


        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
