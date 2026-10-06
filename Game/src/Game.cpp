#include <TuskGame/Game.h>

#include <algorithm>
#include <cmath>

namespace TuskGame {

    DemoGame::DemoGame(/*std::uint8_t red, std::uint8_t green, std::uint8_t blue*/) noexcept {};

    bool DemoGame::Start(Tusk::Engine&) {
        previousTime_ = currentTime_ = 0.0f;
        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Engine&, float seconds) {
        previousTime_ = currentTime_;
        currentTime_ += seconds;
    }

    void DemoGame::Render(Tusk::Engine& engine) {
       const float time = previousTime_ + (currentTime_ - previousTime_);
        const int glow = static_cast<int>(16.0f * (1.0f + std::sin(time)));
        auto& renderer = engine.GetRenderer();

        renderer.Clear(red_, green_, static_cast<float>(std::min(255, int(blue_) + glow)));

        //renderer.DrawRectangle(Color(255, 0, 0), 50, 50, 50, 50);*/
    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
