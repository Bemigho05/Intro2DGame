#include <SFML/Graphics.hpp>
#include "Engine/GameEngine.hpp"


int main() {
    GameEngine g("assets.txt");
    g.run();

    return 0;
}
