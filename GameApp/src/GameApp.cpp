#include <Tusk/Engine.h>
#include <TuskGame/Game.h>

int main() {
    Tusk::Engine engine;
    if (!engine.Initialise("The Universal Simulation Kit Engine")) return 1;
    TuskGame::DemoGame game/*(24, 48, 85)*/;
    return engine.Run(game);
}
