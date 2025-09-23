#include "AppState.h"

//////////////////////////////////////////////////////////////////////////
//    Additional fonts handling
//////////////////////////////////////////////////////////////////////////
void LoadFonts(AppState& appState) // This is called by runnerParams.callbacks.LoadAdditionalFonts
{
    auto runnerParams = HelloImGui::GetRunnerParams();

    runnerParams->callbacks.defaultIconFont = HelloImGui::DefaultIconFont::FontAwesome6;
    // First, load the default font (the default font should be loaded first)
    HelloImGui::ImGuiDefaultSettings::LoadDefaultFont_WithFontAwesomeIcons();
    // Then load the other fonts
    appState.TitleFont = HelloImGui::LoadFont("fonts/DroidSans.ttf", 18.f);

    HelloImGui::FontLoadingParams fontLoadingParamsEmoji;
    appState.EmojiFont = HelloImGui::LoadFont("fonts/NotoEmoji-Regular.ttf", 24.f, fontLoadingParamsEmoji);

    HelloImGui::FontLoadingParams fontLoadingParamsLargeIcon;
    appState.LargeIconFont = HelloImGui::LoadFont("fonts/fontawesome-webfont.ttf", 24.f, fontLoadingParamsLargeIcon);
#ifdef IMGUI_ENABLE_FREETYPE
    // Found at https://www.colorfonts.wtf/
    HelloImGui::FontLoadingParams fontLoadingParamsColor;
    fontLoadingParamsColor.loadColor = true;
    appState.ColorFont = HelloImGui::LoadFont("fonts/Playbox/Playbox-FREE.otf", 24.f, fontLoadingParamsColor);
#endif
}

void PushFontWithDefaultSize(ImFont* font)
{
    ImGui::PushFont(font, font->LegacySize);
}