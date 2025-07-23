#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>

class Character {
private:
    float x, y;           // Vị trí nhân vật
    float velocityX, velocityY; // Vận tốc di chuyển
    int width, height;    // Kích thước nhân vật
    SDL_Color color;      // Màu sắc nhân vật
    float speed;          // Tốc độ di chuyển
    int health;           // Máu
    int maxHealth;        // Máu tối đa
    bool isAlive;         // Trạng thái sống

public:
    // Constructor
    Character(float startX = 100.0f, float startY = 100.0f, 
              int w = 32, int h = 32, 
              SDL_Color characterColor = {0, 100, 255, 255});

    // Destructor
    ~Character();

    // Phương thức di chuyển
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void stopHorizontal();
    void stopVertical();

    // Phương thức cập nhật
    void update(float deltaTime);
    void handleInput(const SDL_Event& event);

    // Phương thức vẽ
    void render(SDL_Renderer* renderer);
    void renderHealthBar(SDL_Renderer* renderer);

    // Phương thức kiểm tra va chạm
    bool checkCollision(const SDL_Rect& other);
    SDL_Rect getBounds();

    // Phương thức quản lý sức khỏe
    void takeDamage(int damage);
    void heal(int amount);
    bool isDead() const;

    // Getter và Setter
    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY);
    void setSpeed(float newSpeed) { speed = newSpeed; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

    // Phương thức giới hạn trong màn hình
    void keepInBounds(int screenWidth, int screenHeight);
};

#endif // CHARACTER_H
