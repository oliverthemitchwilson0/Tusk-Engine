#pragma once

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
	};

	//struct ActionMap {};

	class Input
	{
	public:
		Input() = default;
		~Input() = default;

		void GetKeyDown(Keys key);

	};
}