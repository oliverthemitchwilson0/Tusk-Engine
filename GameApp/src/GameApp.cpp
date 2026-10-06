#include <Tusk/Engine.h>
#include <TuskGame/Game.h>

int main() {
    Tusk::Engine engine;
    if (!engine.Initialise("The Universal Simulation Kit Engine")) return 1;
    TuskGame::DemoGame game;
    return engine.Run(game);
}
