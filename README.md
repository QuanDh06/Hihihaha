# SDL Character Game

Một game đơn giản sử dụng SDL2 với hệ thống nhân vật và 60 FPS lock.

## Tính năng

- ⚡ **60 FPS Lock**: Đảm bảo game chạy ổn định ở 60 FPS
- 🎮 **Character System**: Class nhân vật hoàn chỉnh với di chuyển và sức khỏe
- 🕹️ **Input Handling**: Điều khiển bằng WASD hoặc phím mũi tên
- ❤️ **Health System**: Hệ thống máu với thanh hiển thị
- 🏃 **Smooth Movement**: Di chuyển mượt mà với physics

## Điều khiển

- **WASD** hoặc **Arrow Keys**: Di chuyển nhân vật
- **1**: Gây damage (test)
- **2**: Hồi máu (test)
- **ESC**: Thoát game

## Files

- `main.cpp`: File chính chứa game loop và SDL setup
- `Character.h`: Header file của class Character
- `Character.cpp`: Implementation của class Character

## Compile

```bash
g++ -o game main.cpp Character.cpp -lSDL2 -lSDL2main
```

## Requirements

- SDL2 Library
- C++11 hoặc mới hơn
- Windows/Linux/MacOS

## Author

Tạo bởi GitHub Copilot
