#pragma once
#include <unordered_map>
#include <SDL2/SDL.h>

namespace engine
{
    // For now all considered
    enum KeyState
    {
        UP,
        DOWN,
        TAP,
        HOLD,
        RELEASE
    };

    class InputSys
    {
    private:
        bool quit_ = false;
        std::unordered_map<SDL_Keycode, KeyState> keyStates_;
        static InputSys *instance_;
        InputSys() {}
        ~InputSys() {}
        InputSys(const InputSys &) = delete;
        InputSys &operator=(const InputSys &) = delete;

    public:
        static InputSys &getInstance()
        {
            static InputSys instance;
            return instance;
        }

        void handleEvents();
        inline bool isQuit() { return quit_; }
        inline KeyState getKeyState(SDL_Keycode key) { return keyStates_[key]; }
    };
}