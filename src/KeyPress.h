#pragma once

#include <OBSE/OBSE.h>
#include <REX/REX.h>
#include <cstdint>

namespace CommandsInput {

inline bool IsKeyPressed(uint32_t keyCode) {
    return (GetAsyncKeyState(keyCode) & 0x8000) != 0;
}

inline bool IsKeyPressedEx(uint32_t keyCode, bool checkShift, bool checkCtrl, bool checkAlt) {
    bool keyPressed = (GetAsyncKeyState(keyCode) & 0x8000) != 0;
    bool shiftPressed = !checkShift || (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool ctrlPressed = !checkCtrl || (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    bool altPressed = !checkAlt || (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    return keyPressed && shiftPressed && ctrlPressed && altPressed;
}

} // namespace CommandsInput 