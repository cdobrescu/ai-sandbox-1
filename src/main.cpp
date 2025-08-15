#include "imgui_bundle/imgui_bundle.h"

int main(int, char **)
{
    HelloImGui::SimpleRunnerParams runnerParams;
    runnerParams.guiFunction = []() {
        ImGui::Text("Hello, world!");
    };
    runnerParams.windowSize = {300, 100};
    runnerParams.windowTitle = "Hello ImGui Bundle";
    ImGuiBundle::Run(runnerParams);
    return 0;
}
