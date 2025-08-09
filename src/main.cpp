#include <Game.h>

Game game(900, 600, 60);

int main() {
    if(game.init() == 0) {
        game.handle();
    }
    return 0;
}