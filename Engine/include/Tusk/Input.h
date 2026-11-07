#pragma once

#include <memory>

namespace Tusk
{
	enum Keys
	{
        A = 4,
        B = 5,
        C = 6,
        D = 7,
        E = 8,
        F = 9,
        G = 10,
        H = 11,
        I = 12,
        J = 13,
        K = 14,
        L = 15,
        M = 16,
        N = 17,
        O = 18,
        P = 19,
        Q = 20,
        R = 21,
        S = 22,
        T = 23,
        U = 24,
        V = 25,
        W = 26,
        X = 27,
        Y = 28,
        Z = 29,

        _1 = 30,
        _2 = 31,
        _3 = 32,
        _4 = 33,
        _5 = 34,
        _6 = 35,
        _7 = 36,
        _8 = 37,
        _9 = 38,
        _0 = 39,

        RETURN = 40,
        ESCAPE = 41,
        BACKSPACE = 42,
        TAB = 43,
        SPACE = 44,

        F1 = 58,
        F2 = 59,
        F3 = 60,
        F4 = 61,
        F5 = 62,
        F6 = 63,
        F7 = 64,
        F8 = 65,
        F9 = 66,
        F10 = 67,
        F11 = 68,
        F12 = 69,

        LCTRL = 224,
        LSHIFT = 225,
	};

    enum MouseButton
    {
        LEFT = 0,
        MIDDLE = 1,
        RIGHT = 2,
    };

    struct MouseEvent
    {
        uint8_t button;
        bool down = false;
    };

	class Input
	{
	public:
		Input();
		~Input();

        void Process();
        void PushMouseEvent(const MouseEvent& event);
		bool GetKeyDown(Keys key);
        bool GetKeyPressed(Keys key);
        bool GetMouseBtnDown(MouseButton btn);
        void GetMouseDelta(float* deltaX, float* deltaY);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
	};
}