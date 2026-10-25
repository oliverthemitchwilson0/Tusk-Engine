#pragma once

#include <Tusk/Engine.h>

//#include <cstdint>

namespace TuskGame {

    //Extends from the Engine's Application class.
    class DemoGame final : public Tusk::Application {
    public:
        DemoGame() noexcept;

        bool Start(Tusk::Engine& engine) override;
        void FixedUpdate(Tusk::Engine& engine, float seconds) override;
        void Render(Tusk::Engine& engine) override;
        void Stop(Tusk::Engine& engine) override;

    private:
        /*SDL_GPUBuffer* object;
        SDL_GPUBuffer* objectIndices;*/

        Tusk::GameObject* object;
        Tusk::GameObject* camera;

        float cubeAngle = 0.f;
        float accumulator = 0.f;
    };

}
