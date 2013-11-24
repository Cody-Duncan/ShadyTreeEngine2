#pragma once

enum MouseButton
{
    Left,
    Right,
    Middle
};

class InputState
{
public:
    InputState(void);
    ~InputState(void);

    
    void Init();
    void updateKey(UINT msg, WPARAM wParam, LPARAM lParam);
    void shift();

    ST_API bool keyDown(WPARAM wParam);
    ST_API bool keyHeld(WPARAM wParam);
    ST_API bool keyPressed(WPARAM wParam);
    ST_API bool keyUp(WPARAM wParam);

    std::vector<bool> KeyboardStateNow;
    std::unordered_map<MouseButton, bool> MouseStateNow;
    Vector2 MousePositionPrev;

    std::vector<bool> KeyboardStatePrev;
    std::unordered_map<MouseButton, bool> MouseStatePrev;
    Vector2 MousePositionNow;
    
};

ST_API extern InputState* gINPUTSTATE;
