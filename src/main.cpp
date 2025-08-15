#include "immapp/immapp.h"

int main(int, char **)
{
    HelloImGui::SimpleRunnerParams runnerParams;
    runnerParams.guiFunction = []() {
        ImGui::Text("Hello, world!");
    };
    runnerParams.windowSize = {300, 100};
    runnerParams.windowTitle = "Hello ImGui Bundle";
    ImmApp::Run(runnerParams);
    return 0;
}
