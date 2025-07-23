#include <iostream>
#include <SDL.h>
#include <chrono>
#include "Character.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello World!";
const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS; // milliseconds per frame

// Class để quản lý FPS lock
class FPSLimiter {
private:
    std::chrono::high_resolution_clock::time_point frameStart;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    int frameCount;
    double currentFPS;
    
public:
    FPSLimiter() : frameCount(0), currentFPS(0.0) {
        frameStart = std::chrono::high_resolution_clock::now();
        lastFrameTime = frameStart;
    }
    
    void waitForNextFrame() {
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        
        if (frameDuration.count() < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameDuration.count());
        }
        
        // Tính FPS hiện tại
        frameCount++;
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime);
        
        if (elapsed.count() >= 1000) { // Cập nhật FPS mỗi giây
            currentFPS = frameCount * 1000.0 / elapsed.count();
            frameCount = 0;
            lastFrameTime = now;
        }
        
        frameStart = std::chrono::high_resolution_clock::now();
    }
    
    double getCurrentFPS() const {
        return currentFPS;
    }
};

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //full screen
    //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

void drawSomething(SDL_Window* window, SDL_Renderer* renderer, Character& player) {
    // Vẽ background objects
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
    SDL_RenderDrawPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // red
    SDL_RenderDrawLine(renderer, 100, 100, 200, 200);
    SDL_Rect filled_rect;
    filled_rect.x = SCREEN_WIDTH - 400;
    filled_rect.y = SCREEN_HEIGHT - 150;
    filled_rect.w = 320;
    filled_rect.h = 100;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
    SDL_RenderFillRect(renderer, &filled_rect);
    
    // Vẽ nhân vật
    player.render(renderer);
}

int main(int argc, char* argv[])
{
    //Khởi tạo môi trường đồ họa
    SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SDL_Renderer* renderer = createRenderer(window);
    
    // Khởi tạo FPS limiter
    FPSLimiter fpsLimiter;
    
    // Tạo nhân vật
    Character player(100.0f, 100.0f, 32, 32, {0, 150, 255, 255});
    
    // Game loop với FPS lock
    bool running = true;
    SDL_Event e;
    int frameCounter = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (running) {
        // Tính deltaTime
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Xử lý events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            
            // Xử lý input cho nhân vật
            player.handleInput(e);
            
            // Test damage/heal với phím số
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_1) {
                    player.takeDamage(10);
                    printf("Player took 10 damage! Health: %d/%d\n", 
                           player.getHealth(), player.getMaxHealth());
                }
                else if (e.key.keysym.sym == SDLK_2) {
                    player.heal(10);
                    printf("Player healed 10! Health: %d/%d\n", 
                           player.getHealth(), player.getMaxHealth());
                }
                else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
        
        // Cập nhật nhân vật
        player.update(deltaTime);
        player.keepInBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Xóa màn hình với màu đen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Vẽ gì đó
        drawSomething(window, renderer, player);
        
        // Hiện bản vẽ ra màn hình
        SDL_RenderPresent(renderer);
        
        // In FPS mỗi 60 frames (1 giây)
        frameCounter++;
        if (frameCounter >= 60) {
            printf("Current FPS: %.2f | Player Position: (%.1f, %.1f) | Health: %d/%d\n", 
                   fpsLimiter.getCurrentFPS(), player.getX(), player.getY(), 
                   player.getHealth(), player.getMaxHealth());
            frameCounter = 0;
        }
        
        // Chờ để duy trì 60 FPS
        fpsLimiter.waitForNextFrame();
    }
    
    quitSDL(window, renderer);
    return 0;
}
