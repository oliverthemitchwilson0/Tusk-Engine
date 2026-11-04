#pragma once

#include <Tusk/Engine.h>

namespace TuskGame 
{
    //Extends from the Engine's Application class.
    class DemoGame final : public Tusk::Application {
    public:
        DemoGame() noexcept;

        bool Start(Tusk::Engine& engine) override;
        void FixedUpdate(Tusk::Input& engine, float seconds) override;
        void Render(Tusk::Renderer& engine) override;
        void Stop(Tusk::Engine& engine) override;

    private:
        //
    };

}
