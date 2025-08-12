#include <Game.h>

// FPS = 0 | Configura para ilimitado
Game game(900, 600, 0);

int main() {
    if(game.init() == 0) {
        game.handle();
    }
    return 0;
}