#include "Character.h"
#include <algorithm>
#include <iostream>

// Constructor
Character::Character(float startX, float startY, int w, int h, SDL_Color characterColor)
    : x(startX), y(startY), width(w), height(h), color(characterColor),
      velocityX(0.0f), velocityY(0.0f), speed(200.0f), 
      health(100), maxHealth(100), isAlive(true) {
}

// Destructor
Character::~Character() {
    // Cleanup nếu cần
}

// Phương thức di chuyển
void Character::moveLeft() {
    velocityX = -speed;
}

void Character::moveRight() {
    velocityX = speed;
}

void Character::moveUp() {
    velocityY = -speed;
}

void Character::moveDown() {
    velocityY = speed;
}

void Character::stopHorizontal() {
    velocityX = 0.0f;
}

void Character::stopVertical() {
    velocityY = 0.0f;
}

// Cập nhật vị trí nhân vật
void Character::update(float deltaTime) {
    if (!isAlive) return;

    // Cập nhật vị trí dựa trên vận tốc và deltaTime
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Giảm vận tốc một chút để tạo cảm giác ma sát
    velocityX *= 0.95f;
    velocityY *= 0.95f;

    // Dừng hoàn toàn nếu vận tốc quá nhỏ
    if (abs(velocityX) < 1.0f) velocityX = 0.0f;
    if (abs(velocityY) < 1.0f) velocityY = 0.0f;
}

// Xử lý input
void Character::handleInput(const SDL_Event& event) {
    if (!isAlive) return;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                moveLeft();
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                moveRight();
                break;
            case SDLK_UP:
            case SDLK_w:
                moveUp();
                break;
            case SDLK_DOWN:
            case SDLK_s:
                moveDown();
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
            case SDLK_RIGHT:
            case SDLK_d:
                stopHorizontal();
                break;
            case SDLK_UP:
            case SDLK_w:
            case SDLK_DOWN:
            case SDLK_s:
                stopVertical();
                break;
        }
    }
}

// Vẽ nhân vật
void Character::render(SDL_Renderer* renderer) {
    if (!isAlive) return;

    // Vẽ nhân vật như một hình chữ nhật màu
    SDL_Rect characterRect = { (int)x, (int)y, width, height };
    
    // Đổi màu nhân vật dựa trên máu
    SDL_Color renderColor = color;
    if (health < maxHealth * 0.3f) {
        // Màu đỏ khi máu thấp
        renderColor = {255, 50, 50, 255};
    } else if (health < maxHealth * 0.6f) {
        // Màu vàng khi máu trung bình
        renderColor = {255, 200, 50, 255};
    }
    
    SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect(renderer, &characterRect);
    
    // Vẽ viền đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &characterRect);
    
    // Vẽ thanh máu
    renderHealthBar(renderer);
}

// Vẽ thanh máu
void Character::renderHealthBar(SDL_Renderer* renderer) {
    if (!isAlive) return;

    int barWidth = width;
    int barHeight = 6;
    int barX = (int)x;
    int barY = (int)y - 10;

    // Thanh nền (đỏ)
    SDL_Rect bgRect = { barX, barY, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 100, 20, 20, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    // Thanh máu (xanh lá)
    float healthPercent = (float)health / (float)maxHealth;
    int currentWidth = (int)(barWidth * healthPercent);
    SDL_Rect healthRect = { barX, barY, currentWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
    SDL_RenderFillRect(renderer, &healthRect);

    // Viền thanh máu
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}

// Kiểm tra va chạm
bool Character::checkCollision(const SDL_Rect& other) {
    SDL_Rect myBounds = getBounds();
    return SDL_HasIntersection(&myBounds, &other);
}

SDL_Rect Character::getBounds() {
    return { (int)x, (int)y, width, height };
}

// Quản lý sức khỏe
void Character::takeDamage(int damage) {
    if (!isAlive) return;
    
    health -= damage;
    if (health <= 0) {
        health = 0;
        isAlive = false;
        std::cout << "Character died!" << std::endl;
    }
}

void Character::heal(int amount) {
    if (!isAlive) return;
    
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

bool Character::isDead() const {
    return !isAlive;
}

// Setter
void Character::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

// Giới hạn trong màn hình
void Character::keepInBounds(int screenWidth, int screenHeight) {
    if (x < 0) {
        x = 0;
        velocityX = 0;
    }
    if (x + width > screenWidth) {
        x = screenWidth - width;
        velocityX = 0;
    }
    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    if (y + height > screenHeight) {
        y = screenHeight - height;
        velocityY = 0;
    }
}
