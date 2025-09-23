#include "hello_imgui/hello_imgui.h"

#ifndef APP_STATE_H 
#define APP_STATE_H

//////////////////////////////////////////////////////////////////////////
//    Our Application State
//////////////////////////////////////////////////////////////////////////
struct AppSettings
{
    HelloImGui::InputTextData motto = HelloImGui::InputTextData(
        "Hello, Dear ImGui\n"
        "Unleash your creativity!\n",
        true, // multiline
        ImVec2(14.f, 3.f) // initial size (in em)
    );
    int value = 10;
};

struct AppState
{
    float f = 0.0f;
    int counter = 0;

    float rocket_launch_time = 0.f;
    float rocket_progress = 0.0f;

    enum class RocketState {
        Init,
        Preparing,
        Launched
    };
    RocketState rocket_state = RocketState::Init;

    AppSettings appSettings; // This values will be stored in the application settings

    ImFont *TitleFont;
    ImFont *ColorFont;
    ImFont *EmojiFont;
    ImFont *LargeIconFont;
};


//////////////////////////////////////////////////////////////////////////
//    Additional fonts handling
//////////////////////////////////////////////////////////////////////////
void LoadFonts(AppState& appState);

void PushFontWithDefaultSize(ImFont* font);

#endif // APP_STATE_H