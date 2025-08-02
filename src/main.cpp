#include <Game.h>

Game game(900, 600, 60);

int main() {
    game.init();
    game.handle();
    return 0;
}