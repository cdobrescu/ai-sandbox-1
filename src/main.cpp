#include "hello_imgui/hello_imgui.h"
#include "AppState.h"
#include "Layout.h"



// The Gui of the status bar
void StatusBarGui(AppState& app_state)
{
    ImGui::Text("Using backend: %s", HelloImGui::GetBackendDescription().c_str());
    ImGui::SameLine();
    if (app_state.rocket_state == AppState::RocketState::Preparing)
    {
        ImGui::Text("  -  Rocket completion: ");
        ImGui::SameLine();
        ImGui::ProgressBar(app_state.rocket_progress, HelloImGui::EmToVec2(7.0f, 1.0f));
    }
}


// The menu gui
void ShowMenuGui(HelloImGui::RunnerParams& runnerParams)
{
    HelloImGui::ShowAppMenu(runnerParams);
    HelloImGui::ShowViewMenu(runnerParams);

    if (ImGui::BeginMenu("My Menu"))
    {
        bool clicked = ImGui::MenuItem("Test me", "", false);
        if (clicked)
        {
            HelloImGui::Log(HelloImGui::LogLevel::Warning, "It works");
        }
        ImGui::EndMenu();
    }
}


void ShowAppMenuItems()
{
    if (ImGui::MenuItem("A Custom app menu item"))
        HelloImGui::Log(HelloImGui::LogLevel::Info, "Clicked on A Custom app menu item");
}



int main(int , char *[])
{
    // Our application state
    AppState appState;

    HelloImGui::RunnerParams runnerParams;
    runnerParams.appWindowParams.windowGeometry.size = {1280, 720};
    runnerParams.appWindowParams.windowTitle = "ImGui";

    //
    // Menu bar
    //
    // Here, we fully customize the menu bar:
    // by setting `showMenuBar` to true, and `showMenu_App` and `showMenu_View` to false,
    // HelloImGui will display an empty menu bar, which we can fill with our own menu items via the callback `ShowMenus`
    runnerParams.imGuiWindowParams.showMenuBar = true;
    runnerParams.imGuiWindowParams.showMenu_App = false;
    runnerParams.imGuiWindowParams.showMenu_View = false;
    // Inside `ShowMenus`, we can call `HelloImGui::ShowViewMenu` and `HelloImGui::ShowAppMenu` if desired
    runnerParams.callbacks.ShowMenus = [&runnerParams]() {ShowMenuGui(runnerParams);};
    // Optional: add items to Hello ImGui default App menu
    runnerParams.callbacks.ShowAppMenuItems = ShowAppMenuItems;


    // Load additional font
    runnerParams.callbacks.LoadAdditionalFonts = [&appState]() { LoadFonts(appState); };

    //
    // Status bar
    //
    // We use the default status bar of Hello ImGui
    // uncomment next line in order to hide the FPS in the status bar
    runnerParams.callbacks.ShowStatus = [&appState]() { StatusBarGui(appState); };
    runnerParams.imGuiWindowParams.showStatusBar = true;
    // runnerParams.imGuiWindowParams.showStatusFps = false;

    // params.imGuiWindowParams.showMenu_App = true;

    //###############################################################################################
    // Part 2: Define the application layout and windows
    //###############################################################################################

    // First, tell HelloImGui that we want full screen dock space (this will create "MainDockSpace")
    runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    // In this demo, we also demonstrate multiple viewports: you can drag windows outside out the main window in order to put their content into new native windows
    runnerParams.imGuiWindowParams.enableViewports = true;
    // // Set the default layout
    runnerParams.dockingParams = CreateDefaultLayout(appState);
    // // Add alternative layouts
    // runnerParams.alternativeDockingLayouts = CreateAlternativeLayouts(appState);

    HelloImGui::Run(runnerParams);
    return 0;
}
