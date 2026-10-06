#pragma once

#include <Tusk/Engine.h>

//#include <cstdint>

namespace TuskGame {

    //Extends from the Engine's Application class.
    class DemoGame final : public Tusk::Application {
    public:
        DemoGame(/*std::uint8_t red, std::uint8_t green, std::uint8_t blue*/) noexcept;

        bool Start(Tusk::Engine& engine) override;
        void FixedUpdate(Tusk::Engine& engine, float seconds) override;
        void Render(Tusk::Engine& engine) override;
        void Stop(Tusk::Engine& engine) override;

    private:
        std::uint8_t red_;
        std::uint8_t green_;
        std::uint8_t blue_;
        float previousTime_ = 0.0f;
        float currentTime_ = 0.0f;
    };

}
