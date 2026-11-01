
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

void testMotor(SDL_Joystick* joy,
               const char* name,
               Uint16 low,
               Uint16 high) {
    std::cout << "\nTesting " << name << "...\n";

    SDL_ClearError();

    int result = SDL_JoystickRumble(
        joy, low, high, 2000
    );

    std::cout << "Result: "
              << (result == 0 ? "SUCCESS" : "FAILED")
              << " (" << result << ")\n";

    if (result != 0)
        std::cout << "Error: " << SDL_GetError() << "\n";

    SDL_Delay(2500);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cerr << SDL_GetError() << "\n";
        return 1;
    }

    SDL_JoystickEventState(SDL_ENABLE);

    std::cout << "SDL: " << SDL_GetRevision() << "\n";
    std::cout << "Connect your PS1 USB adapter now.\n";

    Uint32 start = SDL_GetTicks();

    while (SDL_GetTicks() - start < 15000) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_JOYDEVICEADDED) {
                std::cout << "Joystick connected: "
                          << SDL_JoystickNameForIndex(
                              event.jdevice.which)
                          << "\n";
            }

            if (event.type == SDL_JOYDEVICEREMOVED) {
                std::cout << "Joystick removed.\n";
            }
        }

        SDL_Delay(50);
    }

    int count = SDL_NumJoysticks();

    std::cout << "\nDetected devices: " << count << "\n";

    for (int i = 0; i < count; i++) {
        std::cout << "\nDevice " << i << ": "
                  << SDL_JoystickNameForIndex(i) << "\n";

        SDL_Joystick* joy = SDL_JoystickOpen(i);

        if (!joy) {
            std::cout << "Open failed: "
                      << SDL_GetError() << "\n";
            continue;
        }

        bool supported = SDL_JoystickHasRumble(joy);

        std::cout << "Has rumble: "
                  << (supported ? "YES" : "NO") << "\n";

        if (supported) {
            testMotor(joy, "LOW frequency / Large motor",
                      0xFFFF, 0);

            testMotor(joy, "HIGH frequency / Small motor",
                      0, 0xFFFF);

            testMotor(joy, "BOTH motors",
                      0xFFFF, 0xFFFF);
        }

        SDL_JoystickClose(joy);
    }

    SDL_Quit();
    std::cout << "\nTest completed.\n";

    return 0;
}

